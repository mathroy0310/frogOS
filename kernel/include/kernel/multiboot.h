/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   multiboot.h                                  ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:34:16 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 02:58:06 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

#define MULTIBOOT_FLAGS_FRAMEBUFFER (1 << 12)

#define MULTIBOOT_FRAMEBUFFER_TYPE_GRAPHICS 1
#define MULTIBOOT_FRAMEBUFFER_TYPE_TEXT 2

struct framebuffer_info_t {
	uint64_t addr;
	uint32_t pitch;
	uint32_t width;
	uint32_t height;
	uint8_t  bpp;
	uint8_t  type;
	uint8_t  color_info[6];
} __attribute__((packed));

struct multiboot_memory_map_t {
	uint32_t size;
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} __attribute__((packed));

// https://www.gnu.org/software/grub/manual/multiboot/multiboot.html#Boot-information-format
struct multiboot_info_t {
	uint32_t           flags;
	uint32_t           mem_lower;
	uint32_t           mem_upper;
	uint32_t           boot_device;
	uint32_t           cmdline;
	uint32_t           mods_count;
	uint32_t           mods_addr;
	uint32_t           syms[4];
	uint32_t           mmap_length;
	uint32_t           mmap_addr;
	uint32_t           drives_length;
	uint32_t           drives_addr;
	uint32_t           config_table;
	uint32_t           boot_loader_name;
	uint32_t           apm_table;
	uint32_t           vbe_control_info;
	uint32_t           vbe_mode_info;
	uint16_t           vbe_mode;
	uint16_t           vbe_interface_seg;
	uint16_t           vbe_interface_off;
	uint16_t           vbe_interface_len;
	framebuffer_info_t framebuffer;
} __attribute__((packed));

extern "C" multiboot_info_t *g_multiboot_info;
extern "C" uint32_t          g_multiboot_magic;