/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Thread.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 22:58:42 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/28 02:09:41 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Function.h>
#include <FROG/NoCopyMove.h>

namespace Kernel {

class Thread {
	FROG_NON_COPYABLE(Thread);
	FROG_NON_MOVABLE(Thread);

  public:
	enum class State {
		NotStarted,
		Running,
		Paused,
		Sleeping,
		Done,
	};

  public:
	Thread(const FROG::Function<void()> &);
	~Thread();

	uint32_t tid() const { return m_tid; }

	void      set_rsp(uintptr_t rsp) { m_rsp = rsp; }
	void      set_rip(uintptr_t rip) { m_rip = rip; }
	void      set_state(State state) { m_state = state; }
	uintptr_t rsp() const { return m_rsp; }
	uintptr_t rip() const { return m_rip; }
	State     state() const { return m_state; }

	const FROG::Function<void()>* function() const { return &m_function; }

  private:
	Thread(uintptr_t rip, uintptr_t func, uintptr_t arg1 = 0, uintptr_t arg2 = 0, uintptr_t arg3 = 0);
	void on_exit();

  private:
	void          *m_stack_base = nullptr;
	State          m_state = State::NotStarted;
	uintptr_t      m_args[4] = {};
	uintptr_t      m_rip = 0;
	uintptr_t      m_rsp = 0;
	const uint32_t m_tid = 0;

	FROG::Function<void()> m_function;
};

} // namespace Kernel