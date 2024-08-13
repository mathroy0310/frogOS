/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Debug.cpp                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 18:20:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 20:43:20 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/Debug.h>
#include <kernel/Serial.h>
#include <kernel/TTY.h>

namespace Debug {
void DumpStackTrace() {
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
}
void putchar(char ch) {
	if (Serial::IsInitialized()) return Serial::putchar(ch);
	if (TTY::IsInitialized()) return TTY::PutCharCurrent(ch);
}

} // namespace Debug