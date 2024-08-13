/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Traits.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 14:23:11 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 20:46:17 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace FROG {

template <typename T> struct RemoveReference {
	using type = T;
};
template <typename T> struct RemoveReference<T &> {
	using type = T;
};
template <typename T> struct RemoveReference<T &&> {
	using type = T;
};

template <typename T> struct RemoveConst {
	using type = T;
};
template <typename T> struct RemoveConst<const T> {
	using type = T;
};

template <bool B, typename T = void> struct EnableIf {};
template <typename T> struct EnableIf<true, T> {
	using type = T;
};

template <typename T, typename S> struct IsSame {
	static constexpr bool value = false;
};
template <typename T> struct IsSame<T, T> {
	static constexpr bool value = true;
};

template <typename T> struct IsLValueReference {
	static constexpr bool value = false;
};
template <typename T> struct IsLValueReference<T &> {
	static constexpr bool value = true;
};

template <typename T> struct Less {
	constexpr bool operator()(const T &lhs, const T &rhs) const {
		return lhs < rhs;
	}
};
template <typename T> struct Equal {
	constexpr bool operator()(const T &lhs, const T &rhs) const {
		return lhs == rhs;
	}
};
template <typename T> struct Greater {
	constexpr bool operator()(const T &lhs, const T &rhs) const {
		return lhs > rhs;
	}
};

} // namespace FROG