/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   PIC.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 11:54:36 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 17:53:10 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/InterruptController.h>

class PIC final : public InterruptController {
  public:
	virtual void EOI(uint8_t) override;
	virtual void EnableIrq(uint8_t) override;
	virtual void GetISR(uint32_t[8]) override;

	static void Remap();
	static void MaskAll();

  private:
	static PIC *Create();
	friend class InterruptController;
};