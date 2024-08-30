/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Memory.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 14:39:02 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 15:57:35 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Memory.h>

void *operator new(size_t size) { return FROG::allocator(size); }
void *operator new[](size_t size) { return FROG::allocator(size); }

void operator delete(void *addr) { FROG::deallocator(addr); }
void operator delete[](void *addr) { FROG::deallocator(addr); }
void operator delete(void *addr, size_t) { FROG::deallocator(addr); }
void operator delete[](void *addr, size_t) { FROG::deallocator(addr); }