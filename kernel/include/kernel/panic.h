/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   panic.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:36 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 23:25:37 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/kprint.h>

namespace Kernel {

template <typename... Args>
__attribute__((__noreturn__)) static void panic(const char *message, Args... args) {
	kprint("\e[31mKernel panic: ");
	kprint(message, args...);
	kprint("\e[m\n");
	asm volatile("cli; hlt");
	__builtin_unreachable();
}

} // namespace Kernel