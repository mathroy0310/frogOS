/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   memmove.c                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:30:35 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 00:53:41 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void *memmove(void *destp, const void *srcp, size_t n) {
	unsigned char       *dest = static_cast<unsigned char *>(destp);
	const unsigned char *src = static_cast<const unsigned char *>(srcp);

	if (dest < src) {
		for (size_t i = 0; i < n; i++)
			dest[i] = src[i];
	} else {
		for (size_t i = 1; i <= n; i++)
			dest[n - i] = src[n - i];
	}

	return destp;
}
