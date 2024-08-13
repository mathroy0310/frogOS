/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   InterruptController.h                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 17:45:41 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 23:39:51 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

class InterruptController {
  public:
	virtual ~InterruptController() {}

	virtual void                eoi(uint8_t) = 0;
	virtual void                enable_irq(uint8_t) = 0;
	virtual bool                is_in_service(uint8_t) = 0;
	static void                 initialize(bool force_pic);
	static InterruptController &get();
};