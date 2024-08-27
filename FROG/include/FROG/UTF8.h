/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UTF8.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 01:48:19 by maroy             #+#    #+#             */
/*   Updated: 2024/08/27 01:48:27 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace FROG
{

	static constexpr uint16_t utf8_to_codepoint(uint8_t* bytes, size_t count)
	{
		if (count > 3)
			return 0xFFFF;

		for (size_t i = 1; i < count; i++)
			if ((bytes[i] & 0xC0) != 0x80)
				return 0xFFFF;

		switch (count)
		{
			case 1: return   bytes[0];
			case 2: return ((bytes[0] & 0x1F) <<  6) |  (bytes[1] & 0x3F);
			case 3: return ((bytes[0] & 0x1F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
		}

		return 0xFFFF;
	}

}