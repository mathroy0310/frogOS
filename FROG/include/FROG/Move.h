/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Move.h                                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:16:43 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 23:31:18 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Traits.h>

namespace FROG {

template <typename T> constexpr remove_reference_t<T> &&move(T &&arg) {
	return static_cast<remove_reference_t<T> &&>(arg);
}

template <typename T> constexpr T &&forward(remove_reference_t<T> &arg) {
	return static_cast<T &&>(arg);
}

template <typename T> constexpr T &&forward(remove_reference_t<T> &&arg) {
	static_assert(!is_lvalue_reference_v<T>);
	return static_cast<T &&>(arg);
}

} // namespace FROG