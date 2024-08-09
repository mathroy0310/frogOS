/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Serial.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:23 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 11:29:39 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Formatter.h>
#include <kernel/PIT.h>

#define dprintln(...)                                                                                                                                   \
	do {                                                                                                                                                \
		FROG::Formatter::print(Serial::serial_putc, "[{5}.{3}] {}:{}: ", PIT::ms_since_boot() / 1000, PIT::ms_since_boot() % 1000, __FILE__, __LINE__); \
		FROG::Formatter::print(Serial::serial_putc, __VA_ARGS__);                                                                                       \
		FROG::Formatter::print(Serial::serial_putc, "\r\n");                                                                                            \
	} while (false)

#define dwarnln(...)                                           \
	do {                                                       \
		FROG::Formatter::print(Serial::serial_putc, "\e[33m"); \
		dprintln(__VA_ARGS__);                                 \
		FROG::Formatter::print(Serial::serial_putc, "\e[m");   \
	} while (false)

#define derrorln(...)                                          \
	do {                                                       \
		FROG::Formatter::print(Serial::serial_putc, "\e[31m"); \
		dprintln(__VA_ARGS__);                                 \
		FROG::Formatter::print(Serial::serial_putc, "\e[m");   \
	} while (false)

namespace Serial {

void initialize();

void serial_putc(char);

} // namespace Serial