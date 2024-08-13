/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Debug.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 18:18:41 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 23:39:16 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Formatter.h>
#include <kernel/PIT.h>

#define dprintln(...)                                                                                                                               \
	do {                                                                                                                                            \
		FROG::Formatter::print(Debug::putchar, "[{5}.{3}] {}:{}:  ", PIT::ms_since_boot() / 1000, PIT::ms_since_boot() % 1000, __FILE__, __LINE__); \
		FROG::Formatter::print(Debug::putchar, __VA_ARGS__);                                                                                        \
		FROG::Formatter::print(Debug::putchar, "\r\n");                                                                                             \
	} while (false)

#define dwarnln(...)                                      \
	do {                                                  \
		FROG::Formatter::print(Debug::putchar, "\e[33m"); \
		dprintln(__VA_ARGS__);                            \
		FROG::Formatter::print(Debug::putchar, "\e[m");   \
	} while (false)

#define derrorln(...)                                     \
	do {                                                  \
		FROG::Formatter::print(Debug::putchar, "\e[31m"); \
		dprintln(__VA_ARGS__);                            \
		FROG::Formatter::print(Debug::putchar, "\e[m");   \
	} while (false)

#define BOCHS_BREAK() asm volatile("xchgw %bx, %bx")

namespace Debug {
void dump_stack_trace();
void putchar(char);
} // namespace Debug