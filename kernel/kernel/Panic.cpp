/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Panic.cpp                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 02:17:13 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 02:45:55 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/Panic.h>

namespace Kernel {

void dump_stacktrace() {
	struct stackframe {
		stackframe *ebp;
		uintptr_t   eip;
	};

	stackframe *frame = (stackframe *) __builtin_frame_address(0);
	FROG::Formatter::print(Serial::serial_putc, "\e[36mStack trace:\r\n");
	while (frame) {
		FROG::Formatter::print(Serial::serial_putc, "    {}\r\n", (void *) frame->eip);
		frame = frame->ebp;
	}
}

} // namespace Kernel