/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Time.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:32:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 23:32:30 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Formatter.h>

#include <stdint.h>

namespace FROG {

struct Time {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	int     year;
};

} // namespace FROG

namespace FROG::Formatter {

template <void (*PUTC_LIKE)(char)>
void print_argument_impl(const Time &time, const ValueFormat &) {
	print<PUTC_LIKE>("{2}:{2}:{2} {2}.{2}.{4}", time.hour, time.minute, time.second, time.day, time.month, time.year);
}

} // namespace FROG::Formatter