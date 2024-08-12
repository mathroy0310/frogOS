/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Memory.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 14:39:02 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 02:50:42 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Memory.h>

namespace std {
enum class align_val_t : size_t {};
}

void *operator new(size_t size) { return FROG::allocator(size); }
void *operator new[](size_t size) { return FROG::allocator(size); }
void *operator new(size_t size, std::align_val_t align) {
	return FROG::allocator_align(size, (size_t) align);
}
void *operator new[](size_t size, std::align_val_t align) {
	return FROG::allocator_align(size, (size_t) align);
}

void operator delete(void *addr) { FROG::deallocator(addr); }
void operator delete[](void *addr) { FROG::deallocator(addr); }
void operator delete(void *addr, size_t) { FROG::deallocator(addr); }
void operator delete[](void *addr, size_t) { FROG::deallocator(addr); }