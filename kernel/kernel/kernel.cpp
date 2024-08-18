/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kernel.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:34:19 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/13 00:18:21 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Memory.h>
#include <FROG/StringView.h>
#include <FROG/Vector.h>
#include <kernel/Debug.h>
#include <kernel/GDT.h>
#include <kernel/IDT.h>
#include <kernel/IO.h>
#include <kernel/Input.h>
#include <kernel/InterruptController.h>
#include <kernel/MMU.h>
#include <kernel/PIC.h>
#include <kernel/PIT.h>
#include <kernel/RTC.h>
#include <kernel/Scheduler.h>
#include <kernel/Serial.h>
#include <kernel/Shell.h>
#include <kernel/TTY.h>
#include <kernel/VesaTerminalDriver.h>
#include <kernel/kmalloc.h>
#include <kernel/kprint.h>
#include <kernel/multiboot.h>

#define DISABLE_INTERRUPTS() asm volatile("cli")
#define ENABLE_INTERRUPTS() asm volatile("sti")

extern "C" const char g_kernel_cmdline[];

using namespace FROG;

struct ParsedCommandLine {
	bool force_pic = false;
	bool disable_serial = false;
};

ParsedCommandLine ParseCommandLine() {
	ParsedCommandLine result;

	if (!(g_multiboot_info->flags & 0x02)) return result;

	const char *start = g_kernel_cmdline;
	const char *current = g_kernel_cmdline;
	while (true) {
		if (!*current || *current == ' ' || *current == '\t') {
			if (current - start == 6 && memcmp(start, "noapic", 6) == 0)
				result.force_pic = true;
			if (current - start == 8 && memcmp(start, "noserial", 8) == 0)
				result.disable_serial = true;
			if (!*current) break;
			start = current + 1;
		}
		current++;
	}

	return result;
}

static TTY *tty1 = nullptr;

extern "C" void kernel_main() {
	using namespace Kernel;

	DISABLE_INTERRUPTS();

	auto cmdline = ParseCommandLine();
	if (!cmdline.disable_serial) Serial::initialize();
	if (g_multiboot_magic != 0x2BADB002) {
		dprintln("Invalid multiboot magic number");
		return;
	}
	dprintln("Serial output initialized");

	kmalloc_initialize();
	dprintln("kmalloc initialized");

	IDT::initialize();
	dprintln("IDT initialized");

	MMU::initialize();
	dprintln("MMU initialized");

	TerminalDriver *terminal_driver = VesaTerminalDriver::create();
	ASSERT(terminal_driver);
	dprintln("VESA initialized");
	tty1 = new TTY(terminal_driver);

	InterruptController::initialize(cmdline.force_pic);
	dprintln("Interrupt controller initialized");

	PIT::initialize();
	dprintln("PIT initialized");
	if (!Input::initialize()) return;
	dprintln("8042 initialized");

	Scheduler::initialize();
	Scheduler& scheduler = Scheduler::get();
	scheduler.add_thread(FROG::Function<void()>([tty1] { Shell(tty1).run(); }));
	// scheduler.add_thread([]() {
	// 	kprintln("\e[32m");
	// 	kprintln("  .d888                             ");
	// 	kprintln(" d88P\"                              ");
	// 	kprintln(" 888                                ");
	// 	kprintln(" 888888888d888 .d88b.  .d88b.       ");
	// 	kprintln(" 888   888P\"  d88\"\"88bd88P\"88b      ");
	// 	kprintln(" 888   888    888  888888  888      ");
	// 	kprintln(" 888   888    Y88..88PY88b 888      ");
	// 	kprintln(" 888   888     \"Y88P\"  \"Y88888      ");
	// 	kprintln("                           888      ");
	// 	kprintln("                      Y8b d88P      ");
	// 	kprintln("                       \"Y88P\"       ");
	// 	kprintln("\e[m");
	// });
	scheduler.start();
	ASSERT(false);
}