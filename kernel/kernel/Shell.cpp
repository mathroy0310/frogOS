/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Shell.cpp                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:34:34 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/13 00:21:27 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Math.h>
#include <FROG/StringView.h>
#include <FROG/Vector.h>
#include <kernel/CPUID.h>
#include <kernel/IO.h>
#include <kernel/Input.h>
#include <kernel/PIT.h>
#include <kernel/RTC.h>
#include <kernel/Shell.h>

#include <ctype.h>

#define TTY_PRINT(...) \
	Formatter::print([this](char c) { m_tty->putchar(c); }, __VA_ARGS__)
#define TTY_PRINTLN(...) \
	Formatter::println([this](char c) { m_tty->putchar(c); }, __VA_ARGS__)

namespace Kernel {
using namespace FROG;

static auto s_default_prompt = "\\[\e[32m\\]user\\[\e[m\\]# "_sv;

Shell::Shell(TTY *tty) : m_tty(tty) {
	Input::register_key_event_callback({&Shell::key_event_callback, this});
	set_prompt(s_default_prompt);
	MUST(m_buffer.push_back(""_sv));
}

void Shell::set_prompt(StringView prompt) {
	m_prompt_length = 0;
	m_prompt = String();

	bool skipping = false;
	for (size_t i = 0; i < prompt.size(); i++) {
		if (i < prompt.size() - 1 && prompt[i] == '\\') {
			if (prompt[i + 1] == '[') skipping = true;
			if (prompt[i + 1] == ']') skipping = false;
			i++;
			continue;
		}

		MUST(m_prompt.push_back(prompt[i]));
		if (!skipping) m_prompt_length++;
	}
}

void Shell::run() {
	TTY_PRINT("{}", m_prompt);
	for (;;) {
		asm volatile("hlt");
		Input::update();
	}
}

Vector<String> Shell::parse_arguments(StringView command) const {
	Vector<String> result;

	while (!command.empty()) {
		while (!command.empty() && isspace(command.front()))
			command = command.substring(1);

		if (command.empty()) break;

		MUST(result.push_back(""_sv));

		char quoted = '\0';
		bool escape = false;
		while (!command.empty()) {
			char ch = command.front();
			switch (ch) {
			case '"':
			case '\'':
				if (!quoted)
					quoted = ch;
				else if (ch == quoted)
					quoted = '\0';
				else
					goto default_case;
				break;
			case '\\':
				if (escape) goto default_case;
				escape = true;
				break;
			default:
			default_case:
				if (isspace(ch) && !quoted && !escape) goto argument_done;
				if (quoted && escape) {
					switch (ch) {
					case 'f':
						MUST(result.back().push_back('\f'));
						break;
					case 'n':
						MUST(result.back().push_back('\n'));
						break;
					case 'r':
						MUST(result.back().push_back('\r'));
						break;
					case 't':
						MUST(result.back().push_back('\t'));
						break;
					case 'v':
						MUST(result.back().push_back('\v'));
						break;
					case '"':
						MUST(result.back().push_back('"'));
						break;
					case '\'':
						MUST(result.back().push_back('\''));
						break;
					case '\\':
						MUST(result.back().push_back('\\'));
						break;
					default:
						char buffer[3]{'\\', ch, '\0'};
						MUST(result.back().append(buffer));
						break;
					}
				} else {
					MUST(result.back().push_back(ch));
				}
				escape = false;
				break;
			}
			command = command.substring(1);
		}
	argument_done:
		continue;
	}

	return result;
}

void Shell::process_command(const Vector<String> &arguments) {
	if (arguments.empty()) {
	} else if (arguments.front() == "date") {
		if (arguments.size() != 1) {
			TTY_PRINTLN("'date' does not support command line arguments");
			return;
		}
		auto time = RTC::get_current_time();
		TTY_PRINTLN("{}", time);
	} else if (arguments.front() == "echo") {
		if (arguments.size() > 1) {
			TTY_PRINT("{}", arguments[1]);
			for (size_t i = 2; i < arguments.size(); i++)
				TTY_PRINT(" {}", arguments[i]);
		}
		TTY_PRINTLN("");
	} else if (arguments.front() == "clear") {
		if (arguments.size() != 1) {
			TTY_PRINTLN("'clear' does not support command line arguments");
			return;
		}
		m_tty->clear();
		m_tty->set_cursor_position(0, 0);
	} else if (arguments.front() == "time") {
		auto new_args = arguments;
		new_args.remove(0);
		auto start = PIT::ms_since_boot();
		process_command(new_args);
		auto duration = PIT::ms_since_boot() - start;
		TTY_PRINTLN("took {} ms", duration);
	} else if (arguments.front() == "memory") {
		if (arguments.size() != 1) {
			TTY_PRINTLN("'memory' does not support command line arguments");
			return;
		}
		kmalloc_dump_info();
	} else if (arguments.front() == "cpuinfo") {
		if (arguments.size() != 1) {
			TTY_PRINTLN("'cpuinfo' does not support command line arguments");
			return;
		}

		uint32_t ecx, edx;
		auto     vendor = CPUID::get_vendor();
		CPUID::get_features(ecx, edx);

		TTY_PRINTLN("Vendor: '{}'", vendor);
		TTY_PRINTLN("64-bit: {}", CPUID::is_64_bit());
		bool first = true;
		for (int i = 0; i < 32; i++)
			if (ecx & ((uint32_t) 1 << i))
				TTY_PRINT("{}{}", first ? (first = false, "") : ", ", CPUID::feature_string_ecx((uint32_t) 1 << i));
		for (int i = 0; i < 32; i++)
			if (edx & ((uint32_t) 1 << i))
				TTY_PRINT("{}{}", first ? (first = false, "") : ", ", CPUID::feature_string_edx((uint32_t) 1 << i));
		if (!first) TTY_PRINTLN("");
	} else if (arguments.front() == "random") {
		if (arguments.size() != 1) {
			TTY_PRINTLN("'random' does not support command line arguments");
			return;
		}
		uint32_t ecx, edx;
		CPUID::get_features(ecx, edx);
		if (!(ecx & CPUID::Features::ECX_RDRND)) {
			TTY_PRINTLN("cpu does not support RDRAND instruction");
			return;
		}

		for (int i = 0; i < 10; i++) {
			uint32_t random;
			asm volatile("rdrand %0" : "=r"(random));
			TTY_PRINTLN("  0x{8H}", random);
		}
	} else if (arguments.front() == "reboot") {
		if (arguments.size() != 1) {
			TTY_PRINTLN("'reboot' does not support command line arguments");
			return;
		}
		uint8_t good = 0x02;
		while (good & 0x02)
			good = IO::inb(0x64);
		IO::outb(0x64, 0xFE);
		asm volatile("cli; hlt");
	} else {
		TTY_PRINTLN("unrecognized command '{}'", arguments.front());
	}
}

void Shell::rerender_buffer() const {
	TTY_PRINT("\e[{}G{}\e[K", m_prompt_length + 1, m_buffer[m_cursor_pos.line]);
}

static uint32_t get_last_lenght(StringView sv) {
	if (sv.size() >= 2 && ((uint8_t) sv[sv.size() - 2] >> 5) == 0b110) return 2;
	if (sv.size() >= 3 && ((uint8_t) sv[sv.size() - 3] >> 4) == 0b1110)
		return 3;
	if (sv.size() >= 4 && ((uint8_t) sv[sv.size() - 4] >> 3) == 0b11110)
		return 4;
	return Math::min<uint32_t>(sv.size(), 1);
}

static uint32_t get_next_lenght(StringView sv) {
	if (sv.size() >= 2 && ((uint8_t) sv[0] >> 5) == 0b110) return 2;
	if (sv.size() >= 3 && ((uint8_t) sv[0] >> 4) == 0b1110) return 3;
	if (sv.size() >= 4 && ((uint8_t) sv[0] >> 3) == 0b11110) return 4;
	return Math::min<uint32_t>(sv.size(), 1);
}

static uint32_t get_unicode_character_count(StringView sv) {
	uint32_t len = 0;
	for (uint32_t i = 0; i < sv.size(); i++) {
		uint8_t ch = sv[i];
		if ((ch >> 5) == 0b110) i += 1;
		if ((ch >> 4) == 0b1110) i += 2;
		if ((ch >> 3) == 0b11110) i += 3;
		len++;
	}
	return len;
}

void Shell::key_event_callback(Input::KeyEvent event) {
	if (!event.pressed) return;

	String &current_buffer = m_buffer[m_cursor_pos.line];

	switch (event.key) {
	case Input::Key::Backspace:
		if (m_cursor_pos.col > 0) {
			TTY_PRINT("\e[D{} ", current_buffer.sv().substring(m_cursor_pos.index));

			uint32_t len =
			    get_last_lenght(current_buffer.sv().substring(0, m_cursor_pos.index));
			m_cursor_pos.index -= len;
			current_buffer.erase(m_cursor_pos.index, len);
			m_cursor_pos.col--;
		}
		break;

	case Input::Key::Enter:
	case Input::Key::NumpadEnter: {
		TTY_PRINTLN("");
		auto arguments = parse_arguments(current_buffer.sv());
		if (!arguments.empty()) {
			process_command(arguments);
			MUST(m_old_buffer.push_back(current_buffer));
			m_buffer = m_old_buffer;
			MUST(m_buffer.push_back(""_sv));
			MUST(m_buffer.back().reserve(128));
			m_cursor_pos.line = m_buffer.size() - 1;
		}
		m_cursor_pos.col = 0;
		m_cursor_pos.index = 0;
		TTY_PRINT("{}", m_prompt);
		break;
	}

	case Input::Key::Escape:
		TTY_PRINTLN("time since boot {} ms", PIT::ms_since_boot());
		break;

	case Input::Key::Tab:
		break;

	case Input::Key::Left:
		if (m_cursor_pos.index > 0) {
			uint32_t len =
			    get_last_lenght(current_buffer.sv().substring(0, m_cursor_pos.index));
			m_cursor_pos.index -= len;
			m_cursor_pos.col--;
		}
		break;

	case Input::Key::Right:
		if (m_cursor_pos.index < current_buffer.size()) {
			uint32_t len =
			    get_next_lenght(current_buffer.sv().substring(m_cursor_pos.index));
			m_cursor_pos.index += len;
			m_cursor_pos.col++;
		}
		break;

	case Input::Key::Up:
		if (m_cursor_pos.line > 0) {
			const auto &new_buffer = m_buffer[m_cursor_pos.line - 1];
			m_cursor_pos.line--;
			m_cursor_pos.index = new_buffer.size();
			m_cursor_pos.col = get_unicode_character_count(new_buffer);
			rerender_buffer();
		}
		break;

	case Input::Key::Down:
		if (m_cursor_pos.line < m_buffer.size() - 1) {
			const auto &new_buffer = m_buffer[m_cursor_pos.line + 1];
			m_cursor_pos.line++;
			m_cursor_pos.index = new_buffer.size();
			m_cursor_pos.col = get_unicode_character_count(new_buffer);
			rerender_buffer();
		}
		break;

	default: {
		const char *utf8 = Input::key_event_to_utf8(event);
		if (utf8) {
			TTY_PRINT("{}{}", utf8, current_buffer.sv().substring(m_cursor_pos.index));
			MUST(current_buffer.insert(utf8, m_cursor_pos.index));
			m_cursor_pos.index += strlen(utf8);
			m_cursor_pos.col++;
		}
		break;
	}
	}

	TTY_PRINT("\e[{}G", m_prompt_length + m_cursor_pos.col + 1);
}

} // namespace Kernel