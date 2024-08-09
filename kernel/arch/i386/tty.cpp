/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   tty.cpp                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 11:58:57 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 02:25:51 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/IO.h>
#include <kernel/Serial.h>
#include <kernel/VESA.h>
#include <kernel/panic.h>
#include <kernel/tty.h>

#include <string.h>

#define BEL 0x07
#define BS 0x08
#define HT 0x09
#define LF 0x0A
#define FF 0x0C
#define CR 0x0D
#define ESC 0x1B

#define CSI '['

namespace TTY {

static uint32_t    terminal_height = 0;
static uint32_t    terminal_width = 0;
static uint32_t    terminal_row = 0;
static uint32_t    terminal_col = 0;
static VESA::Color terminal_fg = VESA::Color::BRIGHT_WHITE;
static VESA::Color terminal_bg = VESA::Color::BLACK;

static char s_ansi_escape_mode = '\0';
static int  s_ansi_escape_index = 0;
static int  s_ansi_escape_nums[2] = {-1, -1};

template <typename T> inline constexpr T max(T a, T b) {
	return a > b ? a : b;
}
template <typename T> inline constexpr T min(T a, T b) {
	return a < b ? a : b;
}
template <typename T> inline constexpr T clamp(T x, T a, T b) {
	return x < a ? a : x > b ? b : x;
}

void initialize() {
	terminal_width = VESA::GetTerminalWidth();
	terminal_height = VESA::GetTerminalHeight();
}

void clear() {
	VESA::Clear(VESA::Color::BLACK);
}

void setcolor(VESA::Color fg, VESA::Color bg) {
	terminal_fg = fg;
	terminal_bg = bg;
}

void clear_line(size_t line) {
	for (size_t x = 0; x < terminal_width; x++)
		VESA::PutEntryAt(' ', x, line, terminal_fg, terminal_bg);
}

static void update_cursor() {
	uint16_t pos = terminal_row * terminal_width + terminal_col;
	IO::outb(0x3D4, 0x0F);
	IO::outb(0x3D5, (uint8_t) (pos & 0xFF));
	IO::outb(0x3D4, 0x0E);
	IO::outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void set_cursor_pos(int x, int y) {
	terminal_row = y;
	terminal_col = x;
	update_cursor();
}

static uint16_t handle_unicode(uint8_t ch) {
	static uint8_t  unicode_left = 0;
	static uint16_t codepoint = 0;

	if (unicode_left) {
		if ((ch >> 6) == 0b10) {
			codepoint = (codepoint << 6) | ch;
			unicode_left--;
			if (unicode_left > 0)
				return 0xFFFF;
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

static void reset_ansi_escape() {
	s_ansi_escape_mode = '\0';
	s_ansi_escape_index = 0;
	s_ansi_escape_nums[0] = -1;
	s_ansi_escape_nums[1] = -1;
}

static void handle_ansi_SGR() {
	switch (s_ansi_escape_nums[0]) {
	case -1:
	case 0:
		terminal_fg = VESA::Color::BRIGHT_WHITE;
		terminal_bg = VESA::Color::BLACK;
		break;

	case 30:
		terminal_fg = VESA::Color::BRIGHT_BLACK;
		break;
	case 31:
		terminal_fg = VESA::Color::BRIGHT_RED;
		break;
	case 32:
		terminal_fg = VESA::Color::BRIGHT_GREEN;
		break;
	case 33:
		terminal_fg = VESA::Color::BRIGHT_YELLOW;
		break;
	case 34:
		terminal_fg = VESA::Color::BRIGHT_BLUE;
		break;
	case 35:
		terminal_fg = VESA::Color::BRIGHT_MAGENTA;
		break;
	case 36:
		terminal_fg = VESA::Color::BRIGHT_CYAN;
		break;
	case 37:
		terminal_fg = VESA::Color::BRIGHT_WHITE;
		break;

	case 40:
		terminal_bg = VESA::Color::BRIGHT_BLACK;
		break;
	case 41:
		terminal_bg = VESA::Color::BRIGHT_RED;
		break;
	case 42:
		terminal_bg = VESA::Color::BRIGHT_GREEN;
		break;
	case 43:
		terminal_bg = VESA::Color::BRIGHT_YELLOW;
		break;
	case 44:
		terminal_bg = VESA::Color::BRIGHT_BLUE;
		break;
	case 45:
		terminal_bg = VESA::Color::BRIGHT_MAGENTA;
		break;
	case 46:
		terminal_bg = VESA::Color::BRIGHT_CYAN;
		break;
	case 47:
		terminal_bg = VESA::Color::BRIGHT_WHITE;
		break;
	}
}

static void handle_ansi_escape(uint16_t c) {
	switch (s_ansi_escape_mode) {
	case '\1': {
		if (c == CSI) {
			s_ansi_escape_mode = CSI;
			return;
		}
		return reset_ansi_escape();
	}

	case CSI: {
		switch (c) {
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
			int &val = s_ansi_escape_nums[s_ansi_escape_index];
			val = (val == -1) ? (c - '0') : (val * 10 + c - '0');
			return;
		}
		case ';':
			s_ansi_escape_index++;
			return;
		case 'A': // Cursor Up
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			terminal_row = max<int32_t>(terminal_row - s_ansi_escape_nums[0], 0);
			return reset_ansi_escape();
		case 'B': // Curson Down
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			terminal_row = min<int32_t>(terminal_row + s_ansi_escape_nums[0], terminal_height - 1);
			return reset_ansi_escape();
		case 'C': // Cursor Forward
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			terminal_col = min<int32_t>(terminal_col + s_ansi_escape_nums[0], terminal_width - 1);
			return reset_ansi_escape();
		case 'D': // Cursor Back
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			terminal_col = max<int32_t>(terminal_col - s_ansi_escape_nums[0], 0);
			return reset_ansi_escape();
		case 'E': // Cursor Next Line
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			terminal_row = min<int32_t>(terminal_row + s_ansi_escape_nums[0], terminal_height - 1);
			terminal_col = 0;
			return reset_ansi_escape();
		case 'F': // Cursor Previous Line
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			terminal_row = max<int32_t>(terminal_row - s_ansi_escape_nums[0], 0);
			terminal_col = 0;
			return reset_ansi_escape();
		case 'G': // Cursor Horizontal Absolute
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			terminal_col = clamp<int32_t>(s_ansi_escape_nums[0] - 1, 0, terminal_width - 1);
			return reset_ansi_escape();
		case 'H': // Cursor Position
			if (s_ansi_escape_nums[0] == -1)
				s_ansi_escape_nums[0] = 1;
			if (s_ansi_escape_nums[1] == -1)
				s_ansi_escape_nums[1] = 1;
			terminal_row = clamp<int32_t>(s_ansi_escape_nums[0] - 1, 0, terminal_height - 1);
			terminal_col = clamp<int32_t>(s_ansi_escape_nums[1] - 1, 0, terminal_width - 1);
			return reset_ansi_escape();
		case 'J': // Erase in Display
			dprintln("Unsupported ANSI CSI character J");
			return reset_ansi_escape();
		case 'K': // Erase in Line
			switch (s_ansi_escape_nums[0]) {
			case -1:
			case 0:
				for (size_t i = terminal_col; i < terminal_width; i++)
					VESA::PutEntryAt(' ', i, terminal_row, terminal_fg, terminal_bg);
				break;
			case 1:
				for (size_t i = 0; i <= terminal_col; i++)
					VESA::PutEntryAt(' ', i, terminal_row, terminal_fg, terminal_bg);
				break;
			case 2:
				for (size_t i = 0; i < terminal_width; i++)
					VESA::PutEntryAt(' ', i, terminal_row, terminal_fg, terminal_bg);
				break;
			}
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
			handle_ansi_SGR();
			return reset_ansi_escape();
		default:
			dprintln("Unsupported ANSI CSI character {}", c);
			return reset_ansi_escape();
		}
	}

	default:
		dprintln("Unsupported ANSI mode");
		return reset_ansi_escape();
	}
}

void putchar(char _c) {
	uint16_t cp = handle_unicode(_c);
	if (cp == 0xFFFF)
		return;
		
	if (s_ansi_escape_mode)
		return handle_ansi_escape(cp);

	// https://en.wikipedia.org/wiki/ANSI_escape_code
	switch (cp) {
	case BEL: // TODO
		break;
	case BS:
		if (terminal_col > 0)
			terminal_col--;
		break;
	case HT:
		terminal_col++;
		while (terminal_col % 8)
			terminal_col++;
		break;
	case LF:
		terminal_col = 0;
		terminal_row++;
		break;
	case FF:
		terminal_row++;
		break;
	case CR:
		terminal_col = 0;
		break;
	case ESC:
		s_ansi_escape_mode = '\1';
		break;
	default:
		VESA::PutEntryAt(cp, terminal_col, terminal_row, terminal_fg, terminal_bg);
		terminal_col++;
		break;
	}

	if (terminal_col >= terminal_width) {
		terminal_col = 0;
		terminal_row++;
	}

	while (terminal_row >= terminal_height) {
		VESA::Scroll();
		clear_line(terminal_height - 1);

		terminal_col = 0;
		terminal_row--;
	}

	update_cursor();
}

void write(const char *data, size_t size) {
	for (size_t i = 0; i < size; i++)
		putchar(data[i]);
}

void writestring(const char *data) {
	while (*data) {
		putchar(*data);
		data++;
	}
}

} // namespace TTY