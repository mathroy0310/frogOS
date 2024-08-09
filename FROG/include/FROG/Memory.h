/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Memory.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:35:30 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 11:34:46 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#if defined(__is_kernel)
#include <kernel/kmalloc.h>
#else
#include <stdlib.h>
#endif

namespace FROG {
#if defined(__is_kernel)
static constexpr void *(&allocator)(size_t) = kmalloc;
static constexpr void *(&allocator_align)(size_t, size_t) = kmalloc;
static constexpr void (&deallocator)(void *) = kfree;
#else
static constexpr void *(&allocator)(size_t) = malloc;
static constexpr void (&deallocator)(void *) = free;
#endif

template <typename T> class OwnPtr {
  public:
	template <typename... Args> OwnPtr(const Args &...args) {
		m_pointer = new T(args...);
	}

	~OwnPtr() {
		delete m_pointer;
	}

  private:
	T *m_pointer = nullptr;
};

} // namespace FROG