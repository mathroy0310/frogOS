/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   strstr.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:33:46 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 01:33:47 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

char *strstr(const char *haystack, const char *needle) {
	for (size_t i = 0; haystack[i]; i++)
		if (memcmp(haystack + i, needle, strlen(needle)) == 0) return (char *) haystack + i;
	return NULL;
}