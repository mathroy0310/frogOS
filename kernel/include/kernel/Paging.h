/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Paging.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 09:29:42 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 09:29:43 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

namespace Paging {

void Initialize();

void MapFramebuffer(uint32_t address);
void MapRSDP(uint32_t address);
void MapAPIC(uint32_t address);

} // namespace Paging