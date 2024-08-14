/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Arch.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 20:53:47 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/13 22:06:28 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define ARCH(arch) (__arch == arch)

#if !defined(__arch) || (__arch != x86_64 && __arch != i386)
#error "Unsupported architecture"
#endif

#if ARCH(x86_64)
#define read_rsp(rsp) asm volatile("movq %%rsp, %0" : "=r"(rsp))
#define read_rbp(rbp) asm volatile("movq %%rbp, %0" : "=r"(rbp))
#else
#define read_rsp(rsp) asm volatile("movl %%esp, %0" : "=r"(rsp))
#define read_rbp(rbp) asm volatile("movl %%ebp, %0" : "=r"(rbp))
#endif