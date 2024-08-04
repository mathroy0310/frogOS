/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   strcpy.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:33:39 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 01:33:40 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char *strcpy(char *__restrict dest, const char *__restrict src) {
	size_t i;
	for (i = 0; src[i]; i++)
		dest[i] = src[i];
	dest[i] = '\0';
	return dest;
}