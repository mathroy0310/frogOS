/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Time.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:32:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 23:43:07 by mathroy0310    `                         */
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
	uint8_t week_day;
	uint8_t day;
	uint8_t month;
	int     year;
};

} // namespace FROG

namespace FROG::Formatter {

template <void (*PUTC_LIKE)(char)>
void print_argument_impl(const Time &time, const ValueFormat &) {
	constexpr const char *week_days[]{"",    "Sun", "Mon", "Tue",
	                                  "Wed", "Thu", "Fri", "Sat"};
	constexpr const char *months[]{"",    "Jan", "Feb", "Mar", "Apr",
	                               "May", "Jun", "Jul", "Aug", "Sep",
	                               "Oct", "Nov", "Dec"};
	print<PUTC_LIKE>("{} {} {} {}:{}:{} GMT+0 {}", week_days[time.week_day], months[time.month], time.day, time.hour, time.minute, time.second, time.year);
}

} // namespace FROG::Formatter