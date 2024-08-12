/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Panic.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:36 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 03:10:22 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/Serial.h>
#include <kernel/TTY.h>
#include <kernel/kprint.h>

#define Panic(...) PanicImpl(__FILE__, __LINE__, __VA_ARGS__)

namespace Kernel {

void dump_stacktrace();

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
	asm volatile("cli");
	for (;;)
		asm volatile("hlt");
	__builtin_unreachable();
}

} // namespace Kernel