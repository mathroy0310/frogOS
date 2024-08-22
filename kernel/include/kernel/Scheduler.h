/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scheduler.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 22:57:03 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/22 11:37:08 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Function.h>
#include <FROG/LinkedList.h>
#include <kernel/Thread.h>

namespace Kernel {

class Scheduler {
	FROG_NON_COPYABLE(Scheduler);
	FROG_NON_MOVABLE(Scheduler);

  public:
	static void       initialize();
	static Scheduler &get();

	const Thread &current_thread() const;

	template <typename... Args>
	[[nodiscard]] FROG::ErrorOr<void> add_thread(const FROG::Function<void(Args...)> &func, Args... args) {
		uintptr_t flags;
		asm volatile("pushf; pop %0" : "=r"(flags));
		asm volatile("cli");
		TRY(m_threads.emplace_back(func, FROG::forward<Args>(args)...));
		if (flags & (1 << 9)) asm volatile("sti");
		return {};
	}

	void reschedule();
	void set_current_thread_sleeping();
	void start();

	static constexpr size_t ms_between_switch = 4;

  private:
	Scheduler() {}
	void switch_thread();

  private:
	FROG::LinkedList<Thread>           m_threads;
	FROG::LinkedList<Thread>::iterator m_current_iterator;
	uint64_t                           m_last_reschedule = 0;

	friend class Thread;
};

} // namespace Kernel