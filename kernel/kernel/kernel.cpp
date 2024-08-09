/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kernel.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:34:19 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 02:26:08 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/StringView.h>
#include <FROG/Vector.h>
#include <kernel/APIC.h>
#include <kernel/GDT.h>
#include <kernel/IDT.h>
#include <kernel/IO.h>
#include <kernel/Keyboard.h>
#include <kernel/PIC.h>
#include <kernel/PIT.h>
#include <kernel/RTC.h>
#include <kernel/Serial.h>
#include <kernel/Shell.h>
#include <kernel/VESA.h>
#include <kernel/kmalloc.h>
#include <kernel/kprint.h>
#include <kernel/multiboot.h>
#include <kernel/panic.h>
#include <kernel/tty.h>

#define DISABLE_INTERRUPTS() asm volatile("cli")
#define ENABLE_INTERRUPTS() asm volatile("sti")

multiboot_info_t *s_multiboot_info;

using namespace FROG;

struct ParsedCommandLine {
	bool force_pic = false;
};

ParsedCommandLine ParseCommandLine(const char *command_line) {
	auto args = MUST(StringView(command_line).Split([](char c) {
		return c == ' ' || c == '\t';
	}));

	ParsedCommandLine result;
	result.force_pic = args.Has("noapic");
	return result;
}

extern "C" void kernel_main(multiboot_info_t *mbi, uint32_t magic) {
	DISABLE_INTERRUPTS();

	Serial::initialize();
	if (magic != 0x2BADB002) {
		dprintln("Invalid multiboot magic number");
		return;
	}

	s_multiboot_info = mbi;

	if (!VESA::PreInitialize()) {
		dprintln("Could not preinitialize VESA");
		return;
	}

	TTY::initialize();

	dprintln("{}", mbi->framebuffer.type);

	kmalloc_initialize();
	
	VESA::Initialize();
	ParsedCommandLine cmdline;
	if (mbi->flags & 0x02)
		cmdline = ParseCommandLine((const char *) mbi->cmdline);

	APIC::Initialize(cmdline.force_pic);
	gdt_initialize();
	IDT::initialize();

	PIT::initialize();
	if (!Keyboard::initialize())
		return;

	ENABLE_INTERRUPTS();
	
	kprintln("\e[32m");
	kprintln("  .d888                             ");
	kprintln(" d88P\"                              ");
	kprintln(" 888                                ");
	kprintln(" 888888888d888 .d88b.  .d88b.       ");
	kprintln(" 888   888P\"  d88\"\"88bd88P\"88b      ");
	kprintln(" 888   888    888  888888  888      ");
	kprintln(" 888   888    Y88..88PY88b 888      ");
	kprintln(" 888   888     \"Y88P\"  \"Y88888      ");
	kprintln("                           888      ");
	kprintln("                      Y8b d88P      ");
	kprintln("                       \"Y88P\"       ");
	kprintln("\e[m");
	// kprintln("☺☻♥♦♣♠•◘○◙♂♀♪♫☼►◄▼▲¶§▬↔↕▲▼⌐√≈≡≤≥░▒▓█▄▌▐▀▒░┼─│┤├┐└┬┴┼▀▐▄▌░▒");
	// kprintln("▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌");
	// auto time = RTC::GetCurrentTime();
	// kprintln("▐  {}  ▌", time);
	// kprintln("▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌");

	auto &shell = Kernel::Shell::Get();
	shell.Run();

	for (;;) {
		asm("hlt");
	}
}