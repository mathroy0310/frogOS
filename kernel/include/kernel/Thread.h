/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Thread.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 22:58:42 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 22:58:44 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Memory.h>

namespace Kernel {

class Thread {
	FROG_NON_COPYABLE(Thread);
	FROG_NON_MOVABLE(Thread);

  public:
	enum class State {
		NotStarted,
		Running,
		Paused,
		Done,
	};

  public:
	Thread(void (*)());
	~Thread();

	uint32_t id() const { return m_id; }

	void      set_rip(uintptr_t rip) { m_rip = rip; }
	void      set_rsp(uintptr_t rsp) { m_rsp = rsp; }
	void      set_state(State state) { m_state = state; }
	uintptr_t rip() const { return m_rip; }
	uintptr_t rsp() const { return m_rsp; }
	State     state() const { return m_state; }

  private:
	static void on_exit();

  private:
	void          *m_stack_base = nullptr;
	State          m_state = State::NotStarted;
	uintptr_t      m_rip = 0;
	uintptr_t      m_rsp = 0;
	const uint32_t m_id = 0;
};

} // namespace Kernel