/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Move.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 01:16:43 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/21 00:45:34 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Traits.h>
#include <stddef.h>

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

inline void *operator new(size_t, void *addr) { return addr; }
inline void *operator new[](size_t, void *addr) { return addr; }