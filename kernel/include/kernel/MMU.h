/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   MMU.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 11:44:09 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 11:44:10 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

class MMU {
  public:
	static void Intialize();
	static MMU &Get();

	void AllocatePage(uintptr_t);
	void AllocateRange(uintptr_t, ptrdiff_t);

  private:
	MMU();

  private:
	uint64_t *m_page_descriptor_pointer_table;
};