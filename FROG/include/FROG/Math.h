/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Math.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 09:48:11 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 11:25:54 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace FROG::Math {

template <typename T> T min(T a, T b) {
	return a < b ? a : b;
}

template <typename T> T max(T a, T b) {
	return a > b ? a : b;
}

template <typename T> T clamp(T x, T min, T max) {
	return x < min ? min : x > max ? max : x;
}
template <typename T> T gcd(T a, T b) {
	T t;
	while (b) {
		t = b;
		b = a % b;
		a = t;
	}
	return a;
}

template <typename T> T lcm(T a, T b) {
	return a / gcd(a, b) * b;
}
} // namespace FROG::Math