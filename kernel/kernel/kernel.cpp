/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kernel.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 00:31:20 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 15:44:11 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/GDT.h>
#include <kernel/IDT.h>
#include <kernel/IO.h>
#include <kernel/Keyboard.h>
#include <kernel/PIC.h>
#include <kernel/PIT.h>
#include <kernel/Serial.h>
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

void on_key_press(Keyboard::Key key, uint8_t modifiers, bool pressed) {
	if (pressed) {
		if (key == Keyboard::Key::Escape) {
			kprint("time since boot: {} ms\n", PIT::ms_since_boot());
			return;
		} else if (key == Keyboard::Key::Backspace) {
			kprint("\b \b");
		} else {
			char ascii = Keyboard::key_to_ascii(key, modifiers);
			if (ascii)
				kprint("{}", ascii);
		}
	}
}

extern "C" void kernel_main(multiboot_info_t *mbi, uint32_t magic) {
	DISABLE_INTERRUPTS();

	s_multiboot_info = mbi;

	if (magic != 0x2BADB002)
		return;

	TTY::initialize();
	Serial::initialize();

	kmalloc_initialize();
	PIC::initialize();
	gdt_initialize();
	IDT::initialize();

	PIT::initialize();
	Keyboard::initialize(on_key_press);

	// printf("Hello from the kernel!\n");
	kprintln("\e[32mHello from the kernel!\e[0m");
	dprintln("\e[32mHello from the kernel!\e[0m");
	

	ENABLE_INTERRUPTS();

	for (;;) {
		asm("hlt");
	}
}
