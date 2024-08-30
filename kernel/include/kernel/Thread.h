/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 22:58:42 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 18:01:44 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Function.h>
#include <FROG/NoCopyMove.h>

namespace Kernel {

class Thread : public FROG::RefCounted<Thread> {
  public:
	using entry_t = void (*)(void *);

  public:
	static FROG::ErrorOr<FROG::RefPtr<Thread>> create(entry_t, void * = nullptr);
	~Thread();

	uint32_t tid() const { return m_tid; }

	void      set_rsp(uintptr_t rsp) { m_rsp = rsp; }
	void      set_rip(uintptr_t rip) { m_rip = rip; }
	uintptr_t rsp() const { return m_rsp; }
	uintptr_t rip() const { return m_rip; }

	void set_started() { m_started = true; }
	bool started() const { return m_started; }

  private:
	Thread();
	FROG::ErrorOr<void> initialize(entry_t, void*);
	void on_exit();

  private:
	void          *m_stack_base = nullptr;
	uintptr_t      m_rip = 0;
	uintptr_t      m_rsp = 0;
	const uint32_t m_tid = 0;
	bool           m_started = false;

	friend class FROG::RefPtr<Thread>;
};

} // namespace Kernel