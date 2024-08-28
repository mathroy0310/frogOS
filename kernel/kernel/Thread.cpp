/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Thread.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 23:12:13 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/28 02:13:19 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <kernel/Arch.h>
#include <kernel/InterruptController.h>
#include <kernel/Scheduler.h>
#include <kernel/Thread.h>
#include <kernel/kmalloc.h>

#define PAGE_SIZE 4096

namespace Kernel {

static uint32_t s_next_tid = 0;

static constexpr size_t thread_stack_size = 16384;

template <size_t size, typename T> static void write_to_stack(uintptr_t &rsp, const T &value) {
	rsp -= size;
	memcpy((void *) rsp, (void *) &value, size);
}

Thread::Thread(const FROG::Function<void()> &function) : m_tid(s_next_tid++), m_function(function) {
	m_stack_base = kmalloc(thread_stack_size, PAGE_SIZE);
	ASSERT(m_stack_base);

	m_rsp = (uintptr_t) m_stack_base + thread_stack_size;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"
	m_rip = (uintptr_t) (void *) &FROG::Function<void()>::operator();
#pragma GCC diagnostic pop

	write_to_stack<sizeof(void *)>(m_rsp, this);
	write_to_stack<sizeof(void *)>(m_rsp, &Thread::on_exit);
}

Thread::~Thread() { kfree(m_stack_base); }

void Thread::on_exit() {
	asm volatile("cli");
	m_state = State::Done;
	Scheduler::get().switch_thread();
	ASSERT(false);
}

} // namespace Kernel