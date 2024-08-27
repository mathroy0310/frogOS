/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Math.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 09:48:11 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/27 02:32:49 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Traits.h>
#include <stddef.h>
#include <stdint.h>

namespace FROG::Math {

template <typename T> inline constexpr T min(T a, T b) { return a < b ? a : b; }

template <typename T> inline constexpr T max(T a, T b) { return a > b ? a : b; }

template <typename T> inline constexpr T clamp(T x, T min, T max) {
	return x < min ? min : x > max ? max : x;
}
template <integral T> inline constexpr T gcd(T a, T b) {
	T t;
	while (b) {
		t = b;
		b = a % b;
		a = t;
	}
	return a;
}

template <integral T> inline constexpr T lcm(T a, T b) { return a / gcd(a, b) * b; }

template <integral T> inline constexpr T div_round_up(T a, T b) { return (a + b - 1) / b; }

template <integral T> inline constexpr T little_endian_to_host(const uint8_t *bytes) {
	T result = 0;
	for (size_t i = 0; i < sizeof(T); i++)
		result |= (T) bytes[i] << (i * 8);
	return result;
}

template <integral T> inline constexpr T big_endian_to_host(const uint8_t *bytes) {
	T result = 0;
	for (size_t i = 0; i < sizeof(T); i++)
		result |= (T) bytes[i] << (8 * (sizeof(T) - i - 1));
	return result;
}

} // namespace FROG::Math