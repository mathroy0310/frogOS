/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Panic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 23:25:36 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/28 01:55:24 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/Debug.h>

#define panic(...) detail::panic_impl(__FILE__, __LINE__, __VA_ARGS__)

namespace Kernel {

namespace detail {
template <typename... Args>
__attribute__((__noreturn__)) static void panic_impl(const char *file, int line, const char *message, Args... args) {
	asm volatile("cli");
	derrorln("Kernel panic at {}:{}", file, line);
	derrorln(message, args...);
	Debug::dump_stack_trace();
	for (;;)
		asm volatile("hlt");
	__builtin_unreachable();
}
} // namespace detail

} // namespace Kernel