/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TTY.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 11:58:57 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/11 01:33:56 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <FROG/ScopeGuard.h>
#include <kernel/Debug.h>
#include <kernel/IO.h>
#include <kernel/LockGuard.h>
#include <kernel/Panic.h>
#include <kernel/TTY.h>
#include <kernel/TerminalDriver.h>
#include <kernel/kmalloc.h>

#include <string.h>

#define BEL 0x07
#define BS 0x08
#define HT 0x09
#define LF 0x0A
#define FF 0x0C
#define CR 0x0D
#define ESC 0x1B

#define CSI '['

template <typename T> inline constexpr T max(T a, T b) { return a > b ? a : b; }
template <typename T> inline constexpr T min(T a, T b) { return a < b ? a : b; }
template <typename T> inline constexpr T clamp(T x, T a, T b) { return x < a ? a : x > b ? b : x; }

static TTY *s_tty = nullptr;

TTY::TTY(TerminalDriver *driver) : m_terminal_driver(driver) {
	m_width = m_terminal_driver->width();
	m_height = m_terminal_driver->height();

	m_buffer = new Cell[m_width * m_height];

	if (s_tty == nullptr) s_tty = this;
}

void TTY::clear() {
	for (uint32_t i = 0; i < m_width * m_height; i++)
		m_buffer[i] = {.foreground = m_foreground, .background = m_background, .character = ' '};
	m_terminal_driver->clear(m_background);
}

void TTY::set_cursor_position(uint32_t x, uint32_t y) {
	static uint32_t last_x = -1;
	static uint32_t last_y = -1;

	if (last_x != uint32_t(-1) && last_y != uint32_t(-1)) render_from_buffer(last_x, last_y);
	m_terminal_driver->set_cursor_position(x, y);
	last_x = m_column = x;
	last_y = m_row = y;
}

void TTY::set_font(const Kernel::Font &font) {
	m_terminal_driver->set_font(font);

	uint32_t new_width = m_terminal_driver->width();
	uint32_t new_height = m_terminal_driver->height();

	if (m_width != new_width || m_height != new_height) {
		Cell *new_buffer = new Cell[new_width * new_height];
		ASSERT(new_buffer);

		for (uint32_t i = 0; i < new_width * m_height; i++)
			new_buffer[i] = {.foreground = m_foreground, .background = m_background, .character = ' '};

		for (uint32_t y = 0; y < FROG::Math::min<uint32_t>(m_height, new_height); y++)
			for (uint32_t x = 0; x < FROG::Math::min<uint32_t>(m_width, new_width); x++)
				new_buffer[y * new_width + x] = m_buffer[y * m_width + x];

		delete[] m_buffer;
		m_buffer = new_buffer;
		m_width = new_width;
		m_height = new_height;
	}

	for (uint32_t y = 0; y < m_height; y++)
		for (uint32_t x = 0; x < m_width; x++)
			render_from_buffer(x, y);
}

static uint16_t handle_unicode(uint8_t ch) {
	static uint8_t  unicode_left = 0;
	static uint16_t codepoint = 0;

	if (unicode_left) {
		if ((ch >> 6) == 0b10) {
			codepoint = (codepoint << 6) | ch;
			unicode_left--;
			if (unicode_left > 0) return 0xFFFF;
			return codepoint;
		} else {
			// invalid utf-8
			unicode_left = 0;
			return 0x00;
		}
	} else {
		if ((ch >> 3) == 0b11110) {
			unicode_left = 3;
			codepoint = ch & 0b00000111;
			return 0xFFFF;
		}
		if ((ch >> 4) == 0b1110) {
			unicode_left = 2;
			codepoint = ch & 0b00001111;
			return 0xFFFF;
		}
		if ((ch >> 5) == 0b110) {
			unicode_left = 1;
			codepoint = ch & 0b00011111;
			return 0xFFFF;
		}
	}

	return ch & 0x7F;
}

void TTY::reset_ansi_escape() {
	m_ansi_state.mode = '\0';
	m_ansi_state.index = 0;
	m_ansi_state.nums[0] = -1;
	m_ansi_state.nums[1] = -1;
}

void TTY::handle_ansi_sgr() {
	switch (m_ansi_state.nums[0]) {
	case -1:
	case 0:
		m_foreground = TerminalColor::BRIGHT_WHITE;
		m_background = TerminalColor::BLACK;
		break;

	case 30:
		m_foreground = TerminalColor::BRIGHT_BLACK;
		break;
	case 31:
		m_foreground = TerminalColor::BRIGHT_RED;
		break;
	case 32:
		m_foreground = TerminalColor::BRIGHT_GREEN;
		break;
	case 33:
		m_foreground = TerminalColor::BRIGHT_YELLOW;
		break;
	case 34:
		m_foreground = TerminalColor::BRIGHT_BLUE;
		break;
	case 35:
		m_foreground = TerminalColor::BRIGHT_MAGENTA;
		break;
	case 36:
		m_foreground = TerminalColor::BRIGHT_CYAN;
		break;
	case 37:
		m_foreground = TerminalColor::BRIGHT_WHITE;
		break;

	case 40:
		m_background = TerminalColor::BRIGHT_BLACK;
		break;
	case 41:
		m_background = TerminalColor::BRIGHT_RED;
		break;
	case 42:
		m_background = TerminalColor::BRIGHT_GREEN;
		break;
	case 43:
		m_background = TerminalColor::BRIGHT_YELLOW;
		break;
	case 44:
		m_background = TerminalColor::BRIGHT_BLUE;
		break;
	case 45:
		m_background = TerminalColor::BRIGHT_MAGENTA;
		break;
	case 46:
		m_background = TerminalColor::BRIGHT_CYAN;
		break;
	case 47:
		m_background = TerminalColor::BRIGHT_WHITE;
		break;
	}
}

void TTY::handle_ansi_escape(uint16_t ch) {
	switch (m_ansi_state.mode) {
	case '\1': {
		if (ch == CSI) {
			m_ansi_state.mode = CSI;
			return;
		}
		return reset_ansi_escape();
	}

	case CSI: {
		switch (ch) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9': {
			int32_t &val = m_ansi_state.nums[m_ansi_state.index];
			val = (val == -1) ? (ch - '0') : (val * 10 + ch - '0');
			return;
		}
		case ';':
			m_ansi_state.index++;
			return;
		case 'A': // Cursor Up
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			m_row = max<int32_t>(m_row - m_ansi_state.nums[0], 0);
			return reset_ansi_escape();
		case 'B': // Curson Down
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			m_row = min<int32_t>(m_row + m_ansi_state.nums[0], m_height - 1);
			return reset_ansi_escape();
		case 'C': // Cursor Forward
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			m_column = min<int32_t>(m_column + m_ansi_state.nums[0], m_width - 1);
			return reset_ansi_escape();
		case 'D': // Cursor Back
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			m_column = max<int32_t>(m_column - m_ansi_state.nums[0], 0);
			return reset_ansi_escape();
		case 'E': // Cursor Next Line
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			m_row = min<int32_t>(m_row + m_ansi_state.nums[0], m_height - 1);
			m_column = 0;
			return reset_ansi_escape();
		case 'F': // Cursor Previous Line
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			m_row = max<int32_t>(m_row - m_ansi_state.nums[0], 0);
			m_column = 0;
			return reset_ansi_escape();
		case 'G': // Cursor Horizontal Absolute
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			m_column = clamp<int32_t>(m_ansi_state.nums[0] - 1, 0, m_width - 1);
			return reset_ansi_escape();
		case 'H': // Cursor Position
			if (m_ansi_state.nums[0] == -1) m_ansi_state.nums[0] = 1;
			if (m_ansi_state.nums[1] == -1) m_ansi_state.nums[1] = 1;
			m_row = clamp<int32_t>(m_ansi_state.nums[0] - 1, 0, m_height - 1);
			m_column = clamp<int32_t>(m_ansi_state.nums[1] - 1, 0, m_width - 1);
			return reset_ansi_escape();
		case 'J': // Erase in Display
			dprintln("Unsupported ANSI CSI character J");
			return reset_ansi_escape();
		case 'K': // Erase in Line
			if (m_ansi_state.nums[0] == -1 || m_ansi_state.nums[0] == 0)
				for (uint32_t i = m_column; i < m_width; i++)
					putchar_at(' ', i, m_row);
			else
				dprintln("Unsupported ANSI CSI character K");
			return reset_ansi_escape();
		case 'S': // Scroll Up
			dprintln("Unsupported ANSI CSI character S");
			return reset_ansi_escape();
		case 'T': // Scroll Down
			dprintln("Unsupported ANSI CSI character T");
			return reset_ansi_escape();
		case 'f': // Horizontal Vertical Position
			dprintln("Unsupported ANSI CSI character f");
			return reset_ansi_escape();
		case 'm':
			handle_ansi_sgr();
			return reset_ansi_escape();
		default:
			dprintln("Unsupported ANSI CSI character {}", ch);
			return reset_ansi_escape();
		}
	}

	default:
		dprintln("Unsupported ANSI mode");
		return reset_ansi_escape();
	}
}

void TTY::render_from_buffer(uint32_t x, uint32_t y) {
	ASSERT(x < m_width && y < m_height);
	const auto &cell = m_buffer[y * m_width + x];
	m_terminal_driver->putchar_at(cell.character, x, y, cell.foreground, cell.background);
}

void TTY::putchar_at(uint16_t ch, uint32_t x, uint32_t y) {
	ASSERT(x < m_width && y < m_height);
	auto &cell = m_buffer[y * m_width + x];
	cell.character = ch;
	cell.foreground = m_foreground;
	cell.background = m_background;
	m_terminal_driver->putchar_at(ch, x, y, m_foreground, m_background);
}

void TTY::putchar(char ch) {
	Kernel::LockGuard guard(m_lock);

	uint16_t cp = handle_unicode(ch);
	if (cp == 0xFFFF) return;

	if (m_ansi_state.mode != 0) {
		handle_ansi_escape(cp);
		set_cursor_position(m_column, m_row);
		return;
	}

	// https://en.wikipedia.org/wiki/ANSI_escape_code
	switch (cp) {
	case BEL: // TODO
		break;
	case BS:
		if (m_column > 0) m_column--;
		break;
	case HT:
		m_column++;
		while (m_column % 8)
			m_column++;
		break;
	case LF:
		m_column = 0;
		m_row++;
		break;
	case FF:
		m_row++;
		break;
	case CR:
		m_column = 0;
		break;
	case ESC:
		m_ansi_state.mode = '\1';
		break;
	default:
		putchar_at(cp, m_column, m_row);
		m_column++;
		break;
	}

	if (m_column >= m_width) {
		m_column = 0;
		m_row++;
	}

	while (m_row >= m_height) {
		// Shift buffer one line up
		memmove(m_buffer, m_buffer + m_width, m_width * (m_height - 1) * sizeof(Cell));
		// Clear last line in buffer
		for (uint32_t x = 0; x < m_width; x++)
			m_buffer[(m_height - 1) * m_width + x] = {.foreground = m_foreground, .background = m_background, .character = ' '};

		// Render the whole buffer to the screen
		for (uint32_t y = 0; y < m_height; y++)
			for (uint32_t x = 0; x < m_width; x++)
				render_from_buffer(x, y);

		m_column = 0;
		m_row--;
	}

	set_cursor_position(m_column, m_row);
}

void TTY::write(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		putchar(data[i]);
}

void TTY::write_string(const char *data) {
	while (*data) {
		putchar(*data);
		data++;
	}
}

void TTY::putchar_current(char ch) {
	ASSERT(s_tty);
	s_tty->putchar(ch);
}

bool TTY::is_initialized() { return s_tty != nullptr; }