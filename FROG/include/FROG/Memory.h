/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Memory.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 23:35:30 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 23:37:28 by mathroy0310    `                         */
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
static constexpr auto &allocator = kmalloc;
static constexpr auto &deallocator = kfree;
#else
static constexpr auto &allocator = malloc;
static constexpr auto &deallocator = free;
#endif
} // namespace FROG