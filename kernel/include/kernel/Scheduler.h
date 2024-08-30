/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scheduler.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 22:57:03 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 17:16:33 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/LinkedList.h>
#include <FROG/Memory.h>
#include <kernel/Thread.h>

namespace Kernel {

class Scheduler {
  public:
	static FROG::ErrorOr<void> initialize();
	static Scheduler          &get();

	void start();
	void reschedule();

	FROG::ErrorOr<void> add_thread(FROG::RefPtr<Thread>);

	void              set_current_thread_sleeping(uint64_t);
	[[noreturn]] void set_current_thread_done();

  private:
	Scheduler() = default;

	FROG::RefPtr<Thread> current_thread();

	void               wake_threads();
	[[nodiscard]] bool save_current_thread();
	void               remove_and_advance_current_thread();
	void               advance_current_thread();
	[[noreturn]] void  execute_current_thread();

  private:
	struct ActiveThread {
		ActiveThread(const FROG::RefPtr<Thread> &thread) : thread(thread) {}
		FROG::RefPtr<Thread> thread;
		uint64_t             padding = 0;
	};

	struct SleepingThread {
		SleepingThread(const FROG::RefPtr<Thread> &thread, uint64_t wake_time)
		    : thread(thread), wake_time(wake_time) {}
		FROG::RefPtr<Thread> thread;
		uint64_t             wake_time;
	};

	FROG::RefPtr<Thread>             m_idle_thread;
	FROG::LinkedList<ActiveThread>   m_active_threads;
	FROG::LinkedList<SleepingThread> m_sleeping_threads;

	FROG::LinkedList<ActiveThread>::iterator m_current_thread;

	uint64_t m_last_reschedule = 0;
};

} // namespace Kernel