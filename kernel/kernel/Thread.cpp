/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Thread.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 23:12:13 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 18:05:04 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <kernel/Arch.h>
#include <kernel/InterruptController.h>
#include <kernel/kmalloc.h>
#include <kernel/Scheduler.h>
#include <kernel/Thread.h>

#define PAGE_SIZE 4096

namespace Kernel
{

	static uint32_t s_next_tid = 0;

	static constexpr size_t thread_stack_size = 16384;

	template<size_t size, typename T>
	static void write_to_stack(uintptr_t& rsp, const T& value)
	{
		rsp -= size;
		memcpy((void*)rsp, (void*)&value, size);
	}

	FROG::ErrorOr<FROG::RefPtr<Thread>> Thread::create(entry_t entry, void* data)
	{
		auto thread = TRY(FROG::RefPtr<Thread>::create());
		TRY(thread->initialize(entry, data));
		return thread;
	}

	Thread::Thread()
		: m_tid(s_next_tid++)
	{}

	FROG::ErrorOr<void> Thread::initialize(entry_t entry, void* data)
	{
		m_stack_base = kmalloc(thread_stack_size, PAGE_SIZE);
		if (m_stack_base == nullptr)
			return FROG::Error::from_errno(ENOMEM);
		m_rsp = (uintptr_t)m_stack_base + thread_stack_size;
		m_rip = (uintptr_t)entry;

		write_to_stack<sizeof(void*)>(m_rsp, this);
		write_to_stack<sizeof(void*)>(m_rsp, &Thread::on_exit);
		write_to_stack<sizeof(void*)>(m_rsp, data);

		return {};
	}

	Thread::~Thread()
	{
		kfree(m_stack_base);
	}

	void Thread::on_exit()
	{
		Scheduler::get().set_current_thread_done();
		ASSERT_NOT_REACHED();
	}

}