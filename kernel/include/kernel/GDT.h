/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   GDT.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:24:52 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 02:28:28 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

namespace GDT {

union SegmentDesriptor {
	struct {
		uint16_t limit_lo;
		uint16_t base_lo;
		uint8_t  base_hi1;

		uint8_t type : 4;
		uint8_t system : 1;
		uint8_t DPL : 2;
		uint8_t present : 1;

		uint8_t limit_hi : 4;
		uint8_t flags : 4;

		uint8_t base_hi2;
	} __attribute__((packed));

	struct {
		uint32_t low;
		uint32_t high;
	} __attribute__((packed));

} __attribute__((packed));

struct GDTR {
	uint16_t size;
	void    *address;
} __attribute__((packed));

extern "C" GDTR             g_gdtr[];
extern "C" SegmentDesriptor g_gdt[];

} // namespace GDT