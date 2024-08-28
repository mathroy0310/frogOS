/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Scheduler.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 22:57:03 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/28 02:08:43 by maroy            ###   ########.fr       */
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

	FROG::ErrorOr<void> add_thread(const FROG::Function<void()> &function);

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