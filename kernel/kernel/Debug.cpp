/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Debug.cpp                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 18:20:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 18:20:21 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/Debug.h>
#include <kernel/Serial.h>
#include <kernel/TTY.h>

namespace Debug {

void putchar(char ch) {
	if (Serial::IsInitialized()) return Serial::putchar(ch);
	if (TTY::IsInitialized()) return TTY::PutCharCurrent(ch);
}

} // namespace Debug