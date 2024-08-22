/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   NoCopyMove.h                                 ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 20:38:39 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 20:38:45 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define FROG_NON_COPYABLE(class)   \
  private:                         \
	class(const class &) = delete; \
	class &operator=(const class &) = delete

#define FROG_NON_MOVABLE(class) \
  private:                      \
	class(class &&) = delete;   \
	class &operator=(class &&) = delete