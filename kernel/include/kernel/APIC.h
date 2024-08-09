/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   APIC.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 01:53:35 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 09:39:23 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

namespace APIC {

void Initialize(bool force_pic = false);
void EOI(uint8_t irq);
void GetISR(uint32_t[8]);
void EnableIRQ(uint8_t irq);

} // namespace APIC