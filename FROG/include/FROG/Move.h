/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Move.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:16:43 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 14:26:05 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Traits.h>

namespace FROG {

template <typename T>
constexpr typename RemoveReference<T>::type &&Move(T &&arg) {
	return static_cast<typename RemoveReference<T>::type &&>(arg);
}

template <typename T>
constexpr T &&Forward(typename RemoveReference<T>::type &arg) {
	return static_cast<T &&>(arg);
}

template <typename T>
constexpr T &&Forward(typename RemoveReference<T>::type &&arg) {
	static_assert(!IsLValueReference<T>::value);
	return static_cast<T &&>(arg);
}

} // namespace FROG