/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Debug.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:20:21 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 16:57:37 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/Debug.h>
#include <kernel/Serial.h>
#include <kernel/TTY.h>

namespace Debug {
void dump_stack_trace() {
	struct stackframe {
		stackframe *rbp;
		uintptr_t   rip;
	};

	stackframe *frame = (stackframe *) __builtin_frame_address(0);
	if (!frame) {
		dprintln("Could not get frame address");
		return;
	}
	uintptr_t first_rip = frame->rip;
	FROG::Formatter::print(Debug::putchar, "\e[36mStack trace:\r\n");
	while (frame) {
		FROG::Formatter::print(Debug::putchar, "    {}\r\n", (void *) frame->rip);
		frame = frame->rbp;

		if (frame && frame->rip == first_rip) {
			derrorln("looping kernel panic :(");
			return;
		}
	}
	FROG::Formatter::print(Debug::putchar, "\e[m");
}
void putchar(char ch) {
	if (Serial::is_initialized()) return Serial::putchar(ch);
	if (TTY::is_initialized()) return TTY::putchar_current(ch);
}

} // namespace Debug