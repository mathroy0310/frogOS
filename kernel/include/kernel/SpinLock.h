/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   SpinLock.h                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 20:51:22 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/13 21:55:41 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/NoCopyMove.h>

namespace Kernel {

class SpinLock {
	FROG_NON_COPYABLE(SpinLock);
	FROG_NON_MOVABLE(SpinLock);

  public:
	SpinLock() = default;
	void lock();
	void unlock();
	bool is_locked() const;

  private:
	int m_lock = 0;
};

} // namespace Kernel