/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:03:32 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 14:03:40 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/String.h>
#include <FROG/Vector.h>
#include <kernel/Thread.h>

namespace Kernel {

class Process : FROG::RefCounted<Process> {
	FROG_NON_COPYABLE(Process);
	FROG_NON_MOVABLE(Process);

  public:
	using entry_t = Thread::entry_t;

  public:
	static FROG::ErrorOr<FROG::RefPtr<Process>> create_kernel(entry_t, void *);
	~Process() {}

	FROG::ErrorOr<void> add_thread(entry_t, void *);
	void                on_thread_exit(Thread &);

	pid_t pid() const { return m_pid; }

	static FROG::RefPtr<Process> current() { return Thread::current()->process(); }

  private:
	Process(pid_t pid) : m_pid(pid) {}

  private:
	pid_t                              m_pid = 0;
	FROG::String                       m_working_directory;
	FROG::Vector<FROG::RefPtr<Thread>> m_threads;

	friend class FROG::RefPtr<Process>;
};

} // namespace Kernel