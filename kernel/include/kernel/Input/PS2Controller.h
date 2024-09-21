/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2Controller.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:37:41 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 15:37:48 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <kernel/Device.h>

namespace Kernel::Input {

class PS2Device : public CharacterDevice {
  public:
	virtual ~PS2Device() {}
	virtual void on_byte(uint8_t) = 0;
};

class PS2Controller {
  public:
	static FROG::ErrorOr<void> initialize();
	static PS2Controller     &get();
	void                      send_byte(const PS2Device *, uint8_t);

  private:
	PS2Controller() = default;
	FROG::ErrorOr<void> initialize_impl();
	FROG::ErrorOr<void> initialize_device(uint8_t);
	FROG::ErrorOr<void> reset_device(uint8_t);
	FROG::ErrorOr<void> set_scanning(uint8_t, bool);
	static void        device0_irq();
	static void        device1_irq();

  private:
	PS2Device *m_devices[2]{nullptr, nullptr};
};

} // namespace Kernel::Input