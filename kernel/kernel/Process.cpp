/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Process.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:06:34 by maroy             #+#    #+#             */
/*   Updated: 2024/09/03 14:06:39 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/StringView.h>
#include <kernel/Process.h>
#include <kernel/Scheduler.h>

namespace Kernel
{

	FROG::ErrorOr<FROG::RefPtr<Process>> Process::create_kernel(entry_t entry, void* data)
	{
		static pid_t next_pid = 1;
		auto process = TRY(FROG::RefPtr<Process>::create(next_pid++));
		TRY(process->add_thread(entry, data));
		TRY(process->m_working_directory.append("/"sv));
		return process;
	}

	FROG::ErrorOr<void> Process::add_thread(entry_t entry, void* data)
	{
		auto thread = TRY(Thread::create(entry, data, this));
		TRY(m_threads.push_back(thread));
		if (auto res = Scheduler::get().add_thread(thread); res.is_error())
		{
			m_threads.pop_back();
			return res;
		}

		dprintln("add thread {} to process {}", thread->tid(), pid());

		return {};
	}

	void Process::on_thread_exit(Thread& thread)
	{
		dprintln("thread {} exited from process", thread.tid(), pid());
	}

}