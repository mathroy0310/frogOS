/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   PIT.cpp                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:25:10 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 17:49:55 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/APIC.h>
#include <kernel/IDT.h>
#include <kernel/IO.h>
#include <kernel/InterruptController.h>
#include <kernel/kprint.h>

#define IRQ_TIMER 0

#define TIMER0_CTL 0x40
#define TIMER1_CTL 0x41
#define TIMER2_CTL 0x42
#define PIT_CTL 0x43

#define SELECT_CHANNEL0 0x00
#define SELECT_CHANNEL1 0x40
#define SELECT_CHANNEL2 0x80

#define ACCESS_HI 0x10
#define ACCESS_LO 0x20

#define MODE_SQUARE_WAVE 0x06

#define BASE_FREQUENCY 1193182
#define TICKS_PER_SECOND 1000

namespace PIT {
static uint64_t s_system_time = 0;

void clock_handle() {
	s_system_time++;
}

uint64_t ms_since_boot() {
	return s_system_time;
}

void initialize() {
	constexpr uint16_t timer_reload = BASE_FREQUENCY / TICKS_PER_SECOND;

	IO::outb(PIT_CTL, SELECT_CHANNEL0 | ACCESS_LO | ACCESS_HI | MODE_SQUARE_WAVE);

	IO::outb(TIMER0_CTL, (timer_reload >> 0) & 0xff);
	IO::outb(TIMER0_CTL, (timer_reload >> 8) & 0xff);

	IDT::register_irq_handler(IRQ_TIMER, clock_handle);

	InterruptController::Get().EnableIrq(IRQ_TIMER);
}

} // namespace PIT
