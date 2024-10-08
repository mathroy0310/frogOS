/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putchar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/04 00:30:17 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/27 02:28:03 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

#if defined(__is_libk)
	#include <kernel/Panic.h>
#else
	#include <stdlib.h>
#endif

int putchar(int c)
{
#if defined(__is_libk)
	Kernel::panic("Please use kprint() instead of stdio");
#else
	abort();
#endif
	return c;
}