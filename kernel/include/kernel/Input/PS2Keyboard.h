/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2Keyboard.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:38:11 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 15:38:28 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/CircularQueue.h>
#include <kernel/Input/KeyEvent.h>
#include <kernel/Input/PS2Controller.h>
#include <kernel/Input/PS2Keymap.h>

namespace Kernel::Input {

class PS2Keyboard final : public PS2Device {
  private:
	enum Command : uint8_t {
		SET_LEDS = 0xED,
		SCANCODE = 0xF0,
		ENABLE_SCANNING = 0xF4,
		DISABLE_SCANNING = 0xF5,
	};
	enum class State {
		Normal,
		WaitingAck,
	};

  public:
	static FROG::ErrorOr<PS2Keyboard *> create(PS2Controller &);
	virtual void                        on_byte(uint8_t) override;
	virtual void                        update() override;
	virtual FROG::ErrorOr<void>         read(FROG::Span<uint8_t>) override;

  private:
	PS2Keyboard(PS2Controller &controller) : m_controller(controller) {}
	FROG::ErrorOr<void> initialize();
	void                append_command_queue(uint8_t);
	void                append_command_queue(uint8_t, uint8_t);
	void                buffer_has_key();
	void                update_leds();

  private:
	PS2Controller                    &m_controller;
	uint8_t                           m_byte_buffer[10];
	uint8_t                           m_byte_index{0};
	uint8_t                           m_modifiers{0};
	FROG::CircularQueue<KeyEvent, 10> m_event_queue;
	FROG::CircularQueue<uint8_t, 10>  m_command_queue;
	PS2Keymap                         m_keymap;
	State                             m_state{State::Normal};
};
} // namespace Kernel::Input