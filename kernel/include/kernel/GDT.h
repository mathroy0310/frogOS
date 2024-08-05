/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   GDT.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:24:52 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 23:24:53 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

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
	};

	struct {
		uint32_t low;
		uint32_t high;
	};

	SegmentDesriptor() : low(0), high(0) {
	}
	SegmentDesriptor(uint32_t base, uint32_t limit, uint8_t access, uint8_t _flags)
	    : low(0), high(0) {
		set_base(base);
		set_limit(limit);

		high |= ((uint16_t) access) << 8;
		flags = _flags;
	}

	void set_base(uint32_t base) {
		base_lo = base & 0xFFFF;
		base_hi1 = (base >> 16) & 0x00FF;
		base_hi2 = (base >> 24) & 0x00FF;
	}

	void set_limit(uint32_t limit) {
		limit_lo = limit & 0xFFFF;
		limit_hi = (limit >> 16) & 0x00FF;
	}

} __attribute__((packed));

void gdt_initialize();
