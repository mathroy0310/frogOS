/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Arch.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 20:53:47 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/22 11:39:30 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define ARCH(arch) (__arch == arch)

#if !defined(__arch) || (__arch != x86_64 && __arch != i386)
#error "Unsupported architecture"
#endif

#if ARCH(x86_64)
#define read_rsp(rsp) asm volatile("movq %%rsp, %0" : "=r"(rsp))
#define push_callee_saved()                                               \
	asm volatile("pushq %rbx; pushq %rbp; pushq %r12; pushq %r13; pushq " \
	             "%r14; pushq %r15")
#define pop_callee_saved()                                                 \
	asm volatile("popq %r15; popq %r14; popq %r13; popq %r12; popq %rbp; " \
	             "popq %rbx")
#else
#define read_rsp(rsp) asm volatile("movl %%esp, %0" : "=r"(rsp))
#define push_callee_saved() asm volatile("pushal")
#define pop_callee_saved() asm volatile("popal")
#endif