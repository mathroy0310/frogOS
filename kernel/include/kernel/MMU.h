/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   MMU.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 11:44:09 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 20:36:16 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

class MMU {
  public:
	static void Intialize();
	static MMU &Get();

	MMU();
	~MMU();
	
	void AllocatePage(uintptr_t);
	void AllocateRange(uintptr_t, ptrdiff_t);

	void UnAllocatePage(uintptr_t);
	void UnAllocateRange(uintptr_t, ptrdiff_t);



  private:
	uint64_t *m_highest_paging_struct;
};