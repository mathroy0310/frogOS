/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   putchar.cpp                                  ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:30:17 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 02:36:42 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#if defined(__is_libk)
#include <kernel/panic.h>
#include <kernel/TTY.h>
#else
#include <stdlib.h>
#endif

int putchar(int c) {
#if defined(__is_libk)
	Kernel::panic("Please use kprint() instead of stdio");
	char ch = (char) c;
	TTY::PutCharCurrent(ch);
#else
	abort();
#endif
	return c;
}