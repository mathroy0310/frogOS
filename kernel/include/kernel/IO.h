/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IO.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 11:54:04 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/28 01:24:59 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <stddef.h>

namespace IO {

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void outw(uint16_t port, uint16_t val) {
	asm volatile("outw %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
	uint16_t ret;
	asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void outl(uint16_t port, uint32_t val) {
	asm volatile("outl %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint32_t inl(uint16_t port) {
	uint32_t ret;
	asm volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void insb(uint16_t port, uint8_t *buffer, size_t count) {
	asm volatile("rep insb" : "+D"(buffer), "+c"(count), "=m"(*buffer) : "Nd"(port) : "memory");
}

static inline void insw(uint16_t port, uint16_t *buffer, size_t count) {
	asm volatile("rep insw" : "+D"(buffer), "+c"(count), "=m"(*buffer) : "Nd"(port) : "memory");
}

static inline void insl(uint16_t port, uint32_t *buffer, size_t count) {
	asm volatile("rep insl" : "+D"(buffer), "+c"(count), "=m"(*buffer) : "Nd"(port) : "memory");
}

static inline void io_wait() { outb(0x80, 0); }

} // namespace IO