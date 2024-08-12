/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Panic.cpp                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 02:17:13 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 02:17:22 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/Panic.h>

namespace Kernel {

void dump_stacktrace() {
	struct stackframe {
		stackframe *ebp;
		uint32_t    eip;
	};

	stackframe *frame;
	asm volatile("movl %%ebp, %0" : "=r"(frame));
	FROG::Formatter::println(Serial::serial_putc, "\e[36mStack trace:");
	while (frame) {
		FROG::Formatter::println(Serial::serial_putc, "    {}", (void *) frame->eip);
		frame = frame->ebp;
	}
}

} // namespace Kernel