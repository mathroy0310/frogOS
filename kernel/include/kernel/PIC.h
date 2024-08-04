/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   PIC.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 11:54:36 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 11:54:36 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

namespace PIC {

void initialize();
void eoi(uint8_t);
void unmask(uint8_t);
void mask(uint8_t);

uint16_t get_isr();

} // namespace PIC