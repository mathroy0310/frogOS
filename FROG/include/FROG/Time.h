/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 01:17:07 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/21 00:33:48 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Formatter.h>

#include <stdint.h>

namespace FROG {

struct Time {
	uint32_t year;
	uint8_t  month;
	uint8_t  day;
	uint8_t  hour;
	uint8_t  minute;
	uint8_t  second;
	uint8_t  week_day;
};

uint64_t  to_unix_time(const FROG::Time &);
FROG::Time from_unix_time(uint64_t);

} // namespace FROG

namespace FROG::Formatter {

template <typename F> void print_argument(F putc, const Time &time, const ValueFormat &) {
	constexpr const char *week_days[]{"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	constexpr const char *months[]{"",    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
	                               "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	print(putc, "{} {} {} {2}:{2}:{2} GMT+0 {4}", week_days[time.week_day], months[time.month], time.day, time.hour, time.minute, time.second, time.year);
}

} // namespace FROG::Formatter