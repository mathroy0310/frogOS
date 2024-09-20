/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SpinLock.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 20:51:22 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/20 14:38:06 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/NoCopyMove.h>

#include <sys/types.h>

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

class RecursiveSpinLock {
	FROG_NON_COPYABLE(RecursiveSpinLock);
	FROG_NON_MOVABLE(RecursiveSpinLock);

  public:
	RecursiveSpinLock() = default;
	void lock();
	void unlock();
	bool is_locked() const;

  private:
	pid_t    m_locker = 0;
	uint32_t m_lock_depth = 0;
	SpinLock m_lock;
};

} // namespace Kernel