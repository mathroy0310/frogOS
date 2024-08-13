/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Serial.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:23 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 23:43:17 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace Serial {

void initialize();
bool is_initialized();

void putchar(char);

} // namespace Serial