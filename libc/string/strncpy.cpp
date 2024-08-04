/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   strncpy.cpp                                  ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:33:43 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 01:33:44 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char *strncpy(char *__restrict dest, const char *__restrict src, size_t n) {
	size_t i;
	for (i = 0; src[i] && i < n; i++)
		dest[i] = src[i];
	for (; i < n; i++)
		dest[i] = '\0';
	return dest;
}