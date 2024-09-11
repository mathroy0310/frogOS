/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Endianness.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 00:57:25 by maroy             #+#    #+#             */
/*   Updated: 2024/09/11 00:57:29 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Traits.h>

#include <stddef.h>

namespace FROG {

template <integral T> struct LittleEndian {
	constexpr operator T() const {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		return raw;
#else
		T result{0};
		for (size_t i = 0; i < sizeof(T); i++)
			result = (result << 8) | ((raw >> (sizeof(T) - i - 1) * 8) & 0xFF);
		return result;
#endif
	}
	T raw;
};

template <integral T> struct BigEndian {
	constexpr operator T() const {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		return raw;
#else
		T result{0};
		for (size_t i = 0; i < sizeof(T); i++)
			result = (result << 8) | ((raw >> (i * 8)) & 0xFF);
		return result;
#endif
	}

  private:
	T raw;
};

} // namespace FROG