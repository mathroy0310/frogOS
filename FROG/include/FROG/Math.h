/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Math.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 09:48:11 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/22 11:18:48 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Traits.h>

namespace FROG::Math {

template <typename T> T min(T a, T b) { return a < b ? a : b; }

template <typename T> T max(T a, T b) { return a > b ? a : b; }

template <typename T> T clamp(T x, T min, T max) {
	return x < min ? min : x > max ? max : x;
}
template <integral T> T gcd(T a, T b) {
	T t;
	while (b) {
		t = b;
		b = a % b;
		a = t;
	}
	return a;
}

template <integral T> T lcm(T a, T b) { return a / gcd(a, b) * b; }

template <integral T> T div_round_up(T a, T b) { return (a + b - 1) / b; }

} // namespace FROG::Math