/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kernel.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:31:20 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 01:30:37 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

// #include <kernel/GDT.h>
#include <kernel/kmalloc.h>
#include <kernel/kprint.h>
#include <kernel/multiboot.h>
#include <kernel/panic.h>
#include <kernel/tty.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DISABLE_INTERRUPTS() asm volatile("cli")
#define ENABLE_INTERRUPTS() asm volatile("sti")

multiboot_info_t *s_multiboot_info;

extern "C" void kernel_main(multiboot_info_t *mbi, uint32_t magic) {
	DISABLE_INTERRUPTS();

	s_multiboot_info = mbi;

	terminal_initialize();

	kmalloc_initialize();
	if (magic != 0x2BADB002)
		Kernel::panic("Invalid magic in multiboot");
	if (!(mbi->flags & (1 << 6)))
		Kernel::panic("Bootloader did not provide memory map");
	for (uint32_t i = 0; i < mbi->mmap_length;) {
		multiboot_memory_map_t *mmmt = (multiboot_memory_map_t *) (mbi->mmap_addr + i);
		if (mmmt->type == 1)
			kprint("Size: {}, Addr: {}, Length: {}, Type: {}\n", mmmt->size,
			       (void *) mmmt->base_addr, (void *) mmmt->length, mmmt->type);

		i += mmmt->size + sizeof(uint32_t);
	}

	// printf("Hello from the kernel!\n");
	kprint("Hello from the kernel!\n");
}