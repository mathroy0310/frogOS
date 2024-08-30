/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InterruptController.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 17:45:41 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 17:22:14 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>

#define DISABLE_INTERRUPTS() asm volatile("cli")
#define ENABLE_INTERRUPTS() asm volatile("sti")

class InterruptController {
  public:
	virtual ~InterruptController() {}

	virtual void                eoi(uint8_t) = 0;
	virtual void                enable_irq(uint8_t) = 0;
	virtual bool                is_in_service(uint8_t) = 0;
	static void                 initialize(bool force_pic);
	static InterruptController &get();
};

bool      interrupts_enabled();