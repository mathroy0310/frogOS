/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   MMU.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 11:44:09 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/13 00:15:56 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

class MMU {
  public:
	static void initialize();
	static MMU &get();

	MMU();
	~MMU();

	void allocate_page(uintptr_t);
	void allocate_range(uintptr_t, ptrdiff_t);

	void unallocate_page(uintptr_t);
	void unallocate_range(uintptr_t, ptrdiff_t);

  private:
	uint64_t *m_highest_paging_struct;
};