/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   ctype.c                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 13:37:17 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 13:37:17 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>

int isalnum(int c) { return isdigit(c) || isalpha(c); }

int isalpha(int c) { return islower(c) || isupper(c); }

int isascii(int c) { return c <= 0x7F; }

int isblank(int c) { return c == ' ' || c == '\t'; }

int iscntrl(int c) { return c < 32 || c == 0x7F; }

int isdigit(int c) { return '0' <= c && c <= '9'; }

int isgraph(int c) { return 0x21 <= c && c <= 0x7E; }

int islower(int c) { return 'a' <= c && c <= 'z'; }

int isprint(int c) { return isgraph(c) || c == ' '; }

int ispunct(int c) { return isgraph(c) && !isalnum(c); }

int isspace(int c) {
	return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

int isupper(int c) { return 'A' <= c && c <= 'Z'; }

int isxdigit(int c) { return isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F'); }