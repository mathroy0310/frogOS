/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CriticalScope.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 17:21:54 by maroy             #+#    #+#             */
/*   Updated: 2024/08/30 17:25:24 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/NoCopyMove.h>

#include <stddef.h>

namespace Kernel {

class CriticalScope {
	FROG_NON_COPYABLE(CriticalScope);
	FROG_NON_MOVABLE(CriticalScope);

  public:
	CriticalScope() { asm volatile("pushf; cli; pop %0" : "=r"(m_flags)::"memory"); }

	~CriticalScope() { asm volatile("push %0; popf" ::"rm"(m_flags) : "memory", "cc"); }

  private:
	size_t m_flags;
};

} // namespace Kernel