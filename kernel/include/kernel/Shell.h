/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Shell.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:45:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 09:55:36 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/String.h>
#include <kernel/Input.h>
#include <kernel/TTY.h>

namespace Kernel {

class Shell {
  public:
	Shell(const Shell &) = delete;

	static Shell &Get();
	void          SetTTY(TTY *tty);

	void Run();

  private:
	Shell();
	void PrintPrompt();
	void ProcessCommand(const FROG::Vector<FROG::StringView> &arguments);
	void KeyEventCallback(Input::KeyEvent);
	void MouseMoveEventCallback(Input::MouseMoveEvent);

  private:
	TTY         *m_tty;
	FROG::String m_buffer;

	struct {
		bool    exists = false;
		int32_t x = 0;
		int32_t y = 0;
	} m_mouse_pos;
};

} // namespace Kernel