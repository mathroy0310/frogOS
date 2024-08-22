/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   memcmp.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:30:29 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 00:53:23 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

int memcmp(const void *s1, const void *s2, size_t n) {
	const unsigned char *a = static_cast<const unsigned char *>(s1);
	const unsigned char *b = static_cast<const unsigned char *>(s2);

	for (size_t i = 0; i < n; i++)
		if (a[i] != b[i]) return a[i] - b[i];

	return 0;
}
