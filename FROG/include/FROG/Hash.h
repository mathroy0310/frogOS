/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 00:23:31 by maroy             #+#    #+#             */
/*   Updated: 2024/08/18 00:23:34 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Traits.h>
#include <stddef.h>
#include <stdint.h>

namespace FROG {

template <typename T> struct hash;

using hash_t = uint32_t;

inline constexpr hash_t u32_hash(uint32_t val) {
	val = ((val >> 16) ^ val) * 0x119de1f3;
	val = ((val >> 16) ^ val) * 0x119de1f3;
	val = ((val >> 16) ^ val);
	return val;
}

inline constexpr hash_t u64_hash(uint64_t val) {
	hash_t low = u32_hash(val);
	hash_t high = u32_hash(val >> 32);
	return low ^ high;
}

template <integral T> struct hash<T> {
	constexpr hash_t operator()(T val) const {
		if constexpr (sizeof(val) <= sizeof(uint32_t)) return u32_hash(val);
		return u64_hash(val);
	}
};

template <pointer T> struct hash<T> {
	constexpr hash_t operator()(T val) const { return hash<uintptr_t>()((uintptr_t) val); }
};

} // namespace FROG