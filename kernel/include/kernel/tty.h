/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   tty.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 23:25:21 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

namespace TTY {

void initialize();
void putchar(char c);
void clear();
void write(const char *data, size_t size);
void writestring(const char *data);
void set_cursor_pos(int x, int y);

} // namespace TTY