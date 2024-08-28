/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Assert.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:00:38 by maroy             #+#    #+#             */
/*   Updated: 2024/08/28 01:13:28 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#if defined(__is_kernel)
#include <kernel/Panic.h>
#define ASSERT(cond)                                            \
	do {                                                        \
		if (!(cond)) Kernel::panic("ASSERT(" #cond ") failed"); \
	} while (false)
#define ASSERT_NOT_REACHED() Kernel::panic("ASSERT_NOT_REACHED() failed")
#else
#error "NOT IMPLEMENTED"
#endif