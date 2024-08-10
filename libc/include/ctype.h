/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   ctype.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 13:37:14 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 13:37:14 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

int isalnum(int);
int isalpha(int);
int isascii(int);
int isblank(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);

__END_DECLS