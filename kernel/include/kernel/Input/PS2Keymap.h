/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PS2Keymap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:38:43 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 15:38:53 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Vector.h>
#include <kernel/Input/KeyEvent.h>

namespace Kernel::Input {

class PS2Keymap {
  public:
	PS2Keymap();
	Key key_for_scancode_and_modifiers(uint32_t, uint8_t);

  private:
	FROG::Vector<Key> m_normal_keymap;
	FROG::Vector<Key> m_shift_keymap;
	FROG::Vector<Key> m_altgr_keymap;
	FROG::Vector<Key> m_extended_keymap;
};

} // namespace Kernel::Input