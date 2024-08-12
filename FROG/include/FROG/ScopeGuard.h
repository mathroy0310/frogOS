/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   ScopeGuard.h                                 ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 03:07:29 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 03:07:34 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Function.h>

namespace FROG {

class ScopeGuard {
  public:
	ScopeGuard(const FROG::Function<void()> &func) : m_func(func) {}
	~ScopeGuard() { m_func(); }

  private:
	FROG::Function<void()> m_func;
};

} // namespace FROG