/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PIT.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 23:25:10 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/22 11:31:17 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/APIC.h>
#include <kernel/IDT.h>
#include <kernel/IO.h>
#include <kernel/InterruptController.h>
#include <kernel/PIT.h>
#include <kernel/Scheduler.h>
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

void irq_handler() {
	s_system_time++;
	Kernel::Scheduler::get().reschedule();
}

uint64_t ms_since_boot() { return s_system_time; }

void initialize() {
	constexpr uint16_t timer_reload = BASE_FREQUENCY / TICKS_PER_SECOND;

	IO::outb(PIT_CTL, SELECT_CHANNEL0 | ACCESS_LO | ACCESS_HI | MODE_SQUARE_WAVE);

	IO::outb(TIMER0_CTL, (timer_reload >> 0) & 0xff);
	IO::outb(TIMER0_CTL, (timer_reload >> 8) & 0xff);

	IDT::register_irq_handler(PIT_IRQ, irq_handler);

	InterruptController::get().enable_irq(PIT_IRQ);
}

void sleep(uint64_t ms) {
	uint64_t end = s_system_time + ms;
	while (s_system_time < end)
		Kernel::Scheduler::get().set_current_thread_sleeping();
}

} // namespace PIT
