/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Scheduler.h                                  ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 22:57:03 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 22:57:11 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/LinkedList.h>
#include <kernel/Thread.h>

namespace Kernel {

class Scheduler {
	FROG_NON_COPYABLE(Scheduler);
	FROG_NON_MOVABLE(Scheduler);

  public:
	static void       Initialize();
	static Scheduler &Get();

	const Thread &CurrentThread() const;

	void AddThread(void (*)());
	void Switch();
	void Start();

	static constexpr size_t ms_between_switch = 4;

  private:
	Scheduler() {}

  private:
	FROG::LinkedList<Thread>           m_threads;
	FROG::LinkedList<Thread>::iterator m_current_iterator;
};

} // namespace Kernel