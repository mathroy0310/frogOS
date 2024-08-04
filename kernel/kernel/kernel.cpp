/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kernel.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:31:20 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 11:20:55 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/GDT.h>
#include <kernel/IDT.h>
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
	if (magic != 0x2BADB002)
		asm volatile("hlt");

	s_multiboot_info = mbi;

	terminal_initialize();

	kmalloc_initialize();
	gdt_initialize();
	idt_initialize();

	// printf("Hello from the kernel!\n");
	kprint("Hello from the kernel!\n");
	asm volatile("int $14");
}