/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScopeGuard.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 03:07:29 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/21 01:23:40 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Function.h>

namespace FROG {

class ScopeGuard {
  public:
	ScopeGuard(const FROG::Function<void()> &func) : m_func(func) {}
	~ScopeGuard() {
		if (m_enabled) m_func();
	}
	void disable() { m_enabled = false; }

  private:
	FROG::Function<void()> m_func;
	bool                   m_enabled{true};
};

} // namespace FROG