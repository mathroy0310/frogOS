/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   abort.cpp                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:30:24 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 10:16:06 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#if defined(__is_libk)
#include <kernel/Panic.h>
#endif

__attribute__((__noreturn__)) void abort(void) {
#if defined(__is_libk)
	Kernel::Panic("abort()");
#else
	printf("abort()\n");
#endif
	while (1)
		;
	__builtin_unreachable();
}