/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Shell.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:45:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 13:45:37 by mathroy0310    `                         */
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
	void SetPrompt(FROG::StringView);
	void Run();

  private:
	void                     ReRenderBuffer() const;
	FROG::Vector<FROG::String> ParseArguments(FROG::StringView) const;
	void                     ProcessCommand(const FROG::Vector<FROG::String> &);
	void                     KeyEventCallback(Input::KeyEvent);
	void                     MouseMoveEventCallback(Input::MouseMoveEvent);

  private:
	TTY                     *m_tty;
	FROG::Vector<FROG::String> m_old_buffer;
	FROG::Vector<FROG::String> m_buffer;
	FROG::String              m_prompt;
	uint32_t                 m_prompt_length = 0;

	struct {
		uint32_t line = 0;
		uint32_t col = 0;
		uint32_t index = 0;
	} m_cursor_pos;

	struct {
		bool    exists = false;
		int32_t x = 0;
		int32_t y = 0;
	} m_mouse_pos;
};

} // namespace Kernel