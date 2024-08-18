#pragma once

#include <FROG/NoCopyMove.h>

namespace Kernel {

template <typename Lock> class LockGuard {
	FROG_NON_COPYABLE(LockGuard);
	FROG_NON_MOVABLE(LockGuard);

  public:
	LockGuard(Lock &lock) : m_lock(lock) { m_lock.lock(); }

	~LockGuard() { m_lock.unlock(); }

  private:
	Lock &m_lock;
};

} // namespace Kernel