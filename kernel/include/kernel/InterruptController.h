/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   InterruptController.h                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 17:45:41 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 17:45:42 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

class InterruptController {
  public:
	virtual ~InterruptController() {}

	virtual void EOI(uint8_t) = 0;
	virtual void EnableIrq(uint8_t) = 0;
	virtual void GetISR(uint32_t[8]) = 0;

	static void                 Initialize(bool force_pic);
	static InterruptController &Get();
};