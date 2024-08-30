/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 01:45:21 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 17:33:33 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/String.h>
#include <FROG/Vector.h>
#include <kernel/Input.h>
#include <kernel/TTY.h>

namespace Kernel {

class Shell {
  public:
	Shell(TTY *);
	Shell(const Shell &) = delete;
	void set_prompt(FROG::StringView);
	void run();

  private:
	void                       rerender_buffer() const;
	FROG::Vector<FROG::String> parse_arguments(FROG::StringView) const;
	FROG::ErrorOr<void>        process_command(const FROG::Vector<FROG::String> &);
	void                       key_event_callback(Input::KeyEvent);

  private:
	TTY                       *m_tty;
	FROG::Vector<FROG::String> m_old_buffer;
	FROG::Vector<FROG::String> m_buffer;
	FROG::String               m_prompt;
	uint32_t                   m_prompt_length = 0;

	struct {
		uint32_t line = 0;
		uint32_t col = 0;
		uint32_t index = 0;
	} m_cursor_pos;
};

} // namespace Kernel