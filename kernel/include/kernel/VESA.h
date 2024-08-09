/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   VESA.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 11:57:54 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 02:57:01 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

namespace VESA

{

enum class Color : uint8_t {
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	YELLOW = 6,
	WHITE = 7,
	BRIGHT_BLACK = 8,
	BRIGHT_BLUE = 9,
	BRIGHT_GREEN = 10,
	BRIGHT_CYAN = 11,
	BRIGHT_RED = 12,
	BRIGHT_MAGENTA = 13,
	BRIGHT_YELLOW = 14,
	BRIGHT_WHITE = 15,

};

bool     Initialize();
void     PutCharAt(uint16_t, uint32_t, uint32_t, Color, Color);
void     Clear(Color);
void     SetCursorPosition(uint32_t, uint32_t, Color);
uint32_t GetTerminalWidth();
uint32_t GetTerminalHeight();

} // namespace VESA