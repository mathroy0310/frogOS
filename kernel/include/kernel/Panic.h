/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Panic.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:36 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 13:59:38 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/Serial.h>
#include <kernel/TTY.h>
#include <kernel/VESA.h>
#include <kernel/kprint.h>

#define Panic(...) PanicImpl(__FILE__, __LINE__, __VA_ARGS__)

namespace Kernel {

static void dump_stacktrace() {
	struct stackframe {
		stackframe *ebp;
		uint32_t    eip;
	};

	stackframe *frame;
	asm volatile("movl %%ebp, %0" : "=r"(frame));
	FROG::Formatter::println(Serial::serial_putc, "\e[36mStack trace:");
	while (frame) {
		FROG::Formatter::println(Serial::serial_putc, "    0x{8H}", frame->eip);
		frame = frame->ebp;
	}
}

template <typename... Args>
__attribute__((__noreturn__)) static void PanicImpl(const char *file, int line, const char *message, Args... args) {
	derrorln("Kernel panic at {}:{}", file, line);
	derrorln(message, args...);
	dump_stacktrace();
	if (TTY::IsInitialized()) {
		kprint("\e[31mKernel panic at {}:{}\n", file, line);
		kprint(message, args...);
		kprint("\e[m\n");
	}
	asm volatile("cli; hlt");
	__builtin_unreachable();
}

} // namespace Kernel