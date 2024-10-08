/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterruptController.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 17:48:10 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 17:22:29 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <kernel/APIC.h>
#include <kernel/InterruptController.h>
#include <kernel/PIC.h>

static InterruptController *s_instance = nullptr;

InterruptController &InterruptController::get() {
	ASSERT(s_instance);
	return *s_instance;
}

void InterruptController::initialize(bool force_pic) {
	ASSERT(s_instance == nullptr);

	PIC::mask_all();
	PIC::remap();

	if (!force_pic) s_instance = APIC::create();
	if (s_instance) return;
	dprintln("Using PIC instead of APIC");
	s_instance = PIC::create();
}

bool interrupts_enabled()
{
	uintptr_t flags;
	asm volatile("pushf; pop %0" : "=r"(flags) :: "memory");
	return flags & (1 << 9);
}