/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Scheduler.h                                  ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 22:57:03 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 23:56:55 by mathroy0310    `                         */
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
	void add_thread(const FROG::Function<void(Args...)> &func, Args... args) {
		uintptr_t flags;
		asm volatile("pushf; pop %0" : "=r"(flags));
		asm volatile("cli");
		MUST(m_threads.emplace_back(func, FROG::forward<Args>(args)...));
		if (flags & (1 << 9)) asm volatile("sti");
	}

	void switch_thread();
	void start();

	static constexpr size_t ms_between_switch = 1;

  private:
	Scheduler() {}

  private:
	FROG::LinkedList<Thread>           m_threads;
	FROG::LinkedList<Thread>::iterator m_current_iterator;
};

} // namespace Kernel