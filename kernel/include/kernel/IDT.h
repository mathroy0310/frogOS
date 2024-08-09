/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   IDT.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 11:06:06 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 01:53:25 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

constexpr uint8_t IRQ_VECTOR_BASE = 0x20;

namespace IDT {
void initialize();
void register_irq_handler(uint8_t irq, void (*f)());
} // namespace IDT