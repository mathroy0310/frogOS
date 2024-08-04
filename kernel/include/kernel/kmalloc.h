/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kmalloc.h                                    ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:34:12 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 15:07:55 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

void kmalloc_initialize();
void kmalloc_dump_nodes();

void *kmalloc(size_t);
void  kfree(void *);

inline void *operator new(size_t size) {
	return kmalloc(size);
}
inline void *operator new[](size_t size) {
	return kmalloc(size);
}

inline void operator delete(void *addr) {
	kfree(addr);
}
inline void operator delete[](void *addr) {
	kfree(addr);
}
inline void operator delete(void *addr, size_t) {
	kfree(addr);
}
inline void operator delete[](void *addr, size_t) {
	kfree(addr);
}