/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scheduler.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 23:00:36 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/03 14:06:50 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/Arch.h>
#include <kernel/Attributes.h>
#include <kernel/CriticalScope.h>
#include <kernel/InterruptController.h>
#include <kernel/Process.h>
#include <kernel/Scheduler.h>

#if 1
#define VERIFY_STI() ASSERT(interrupts_enabled())
#define VERIFY_CLI() ASSERT(!interrupts_enabled())
#else
#define VERIFY_STI()
#define VERIFY_CLI()
#endif

namespace Kernel {

extern "C" void      start_thread(uintptr_t rsp, uintptr_t rip);
extern "C" void      continue_thread(uintptr_t rsp, uintptr_t rip);
extern "C" uintptr_t read_rip();

static Scheduler *s_instance = nullptr;

FROG::ErrorOr<void> Scheduler::initialize() {
	ASSERT(s_instance == nullptr);
	s_instance = new Scheduler();
	ASSERT(s_instance);
	s_instance->m_idle_thread = TRY(Thread::create([](void *) {
		for (;;)
			asm volatile("hlt");
	}));
	return {};
}

Scheduler &Scheduler::get() {
	ASSERT(s_instance);
	return *s_instance;
}

void Scheduler::start() {
	VERIFY_CLI();
	ASSERT(!m_active_threads.empty());
	m_current_thread = m_active_threads.begin();
	execute_current_thread();
	ASSERT_NOT_REACHED();
}

FROG::RefPtr<Thread> Scheduler::current_thread() {
	return m_current_thread ? m_current_thread->thread : m_idle_thread;
}

void Scheduler::reschedule() {
	VERIFY_CLI();
	ASSERT(InterruptController::get().is_in_service(PIT_IRQ));
	InterruptController::get().eoi(PIT_IRQ);

	if (PIT::ms_since_boot() <= m_last_reschedule) return;
	m_last_reschedule = PIT::ms_since_boot();

	wake_threads();

	if (save_current_thread()) return;
	advance_current_thread();
	execute_current_thread();
	ASSERT_NOT_REACHED();
}

void Scheduler::wake_threads() {
	VERIFY_CLI();

	uint64_t current_time = PIT::ms_since_boot();
	while (!m_sleeping_threads.empty() && m_sleeping_threads.front().wake_time <= current_time) {
		auto thread = m_sleeping_threads.front().thread;
		m_sleeping_threads.remove(m_sleeping_threads.begin());

		// This should work as we released enough memory from sleeping thread
		static_assert(sizeof(ActiveThread) == sizeof(SleepingThread));
		MUST(m_active_threads.emplace_back(thread));
		thread.clear();
	}
}

FROG::ErrorOr<void> Scheduler::add_thread(FROG::RefPtr<Thread> thread) {
	Kernel::CriticalScope critical;
	TRY(m_active_threads.emplace_back(thread));
	return {};
}

void Scheduler::advance_current_thread() {
	VERIFY_CLI();

	if (m_active_threads.empty()) {
		m_current_thread = {};
		return;
	}
	if (!m_current_thread || ++m_current_thread == m_active_threads.end())
		m_current_thread = m_active_threads.begin();
}

void Scheduler::remove_and_advance_current_thread() {
	VERIFY_CLI();

	ASSERT(m_current_thread);

	if (m_active_threads.size() == 1) {
		m_active_threads.remove(m_current_thread);
		m_current_thread = {};
	} else {
		auto temp = m_current_thread;
		advance_current_thread();
		m_active_threads.remove(temp);
	}
}

// NOTE: this is declared always inline, so we don't corrupt the stack
//       after getting the rsp
ALWAYS_INLINE bool Scheduler::save_current_thread() {
	VERIFY_CLI();

	uintptr_t rsp, rip;
	push_callee_saved();
	if (!(rip = read_rip())) {
		pop_callee_saved();
		return true;
	}
	read_rsp(rsp);

	auto current = current_thread();
	current->set_rip(rip);
	current->set_rsp(rsp);
	return false;
}

void Scheduler::execute_current_thread() {
	VERIFY_CLI();

	auto &current = *current_thread();

	if (current.started()) {
		continue_thread(current.rsp(), current.rip());
	} else {
		current.set_started();
		start_thread(current.rsp(), current.rip());
	}

	ASSERT_NOT_REACHED();
}

#pragma GCC push_options
#pragma GCC optimize("O0")
void        Scheduler::set_current_thread_sleeping(uint64_t wake_time) {
    VERIFY_STI();
    DISABLE_INTERRUPTS();

    ASSERT(m_current_thread);

    auto sleeping = m_current_thread->thread;

    if (save_current_thread()) {
        ENABLE_INTERRUPTS();
        return;
    }
    remove_and_advance_current_thread();

    auto it = m_sleeping_threads.begin();
    for (; it != m_sleeping_threads.end(); it++)
        if (wake_time <= it->wake_time) break;

    // This should work as we released enough memory from active thread
    static_assert(sizeof(ActiveThread) == sizeof(SleepingThread));
    MUST(m_sleeping_threads.emplace(it, sleeping, wake_time));
    sleeping.clear();

    execute_current_thread();
    ASSERT_NOT_REACHED();
}
#pragma GCC pop_options

void Scheduler::set_current_thread_done() {
	VERIFY_STI();
	DISABLE_INTERRUPTS();

	ASSERT(m_current_thread);
	remove_and_advance_current_thread();

	execute_current_thread();
	ASSERT_NOT_REACHED();
}

} // namespace Kernel
