/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   memset.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:30:37 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 00:53:59 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void *memset(void *s, int c, size_t n) {
	unsigned char *p = static_cast<unsigned char *>(s);
	for (size_t i = 0; i < n; i++)
		p[i] = c;
	return s;
}