/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   memcpy.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:30:32 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 00:53:33 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void *memcpy(void *__restrict dstp, const void *__restrict srcp, size_t n) {
	unsigned char       *dst = static_cast<unsigned char *>(dstp);
	const unsigned char *src = static_cast<const unsigned char *>(srcp);
	for (size_t i = 0; i < n; i++)
		dst[i] = src[i];
	return dstp;
}