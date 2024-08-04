/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   IDT.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 11:06:06 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 11:08:13 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

union GateDescriptor {
	struct {
		uint16_t offset_lo;
		uint16_t selector;
		uint8_t  reserved;
		uint8_t  type : 4;
		uint8_t  zero : 1;
		uint8_t  dpl : 2;
		uint8_t  present : 1;
		uint16_t offset_hi;
	};

	struct {
		uint32_t low;
		uint32_t high;
	};
} __attribute__((packed));

void idt_initialize();