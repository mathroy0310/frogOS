/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   PIC.h                                        ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 11:54:36 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 23:40:34 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/InterruptController.h>

class PIC final : public InterruptController {
  public:
	virtual void eoi(uint8_t) override;
	virtual void enable_irq(uint8_t) override;
	virtual bool is_in_service(uint8_t) override;

	static void remap();
	static void mask_all();

  private:
	static PIC *create();
	friend class InterruptController;
};