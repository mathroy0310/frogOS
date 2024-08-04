/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   stdio.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:33:20 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 01:33:21 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/cdefs.h>

#define EOF (-1)

__BEGIN_DECLS

int printf(const char* __restrict, ...);
int putchar(int);
int puts(const char*);

__END_DECLS
