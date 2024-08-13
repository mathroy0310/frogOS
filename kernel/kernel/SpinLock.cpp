/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   SpinLock.cpp                                 ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 20:51:02 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 20:51:03 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/SpinLock.h>

namespace Kernel {

extern "C" void spinlock_lock_asm(int *);
extern "C" void spinlock_unlock_asm(int *);

void SpinLock::lock() { spinlock_lock_asm(&m_lock); }

void SpinLock::unlock() { spinlock_unlock_asm(&m_lock); }

} // namespace Kernel