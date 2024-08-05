/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Shell.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:45:21 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/05 11:35:45 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/String.h>
#include <kernel/Keyboard.h>

namespace Kernel {

class Shell {
public:
  Shell(const Shell &) = delete;

  static Shell &Get();

  void Run();

private:
  Shell();
  void PrintPrompt();
  void ProcessCommand(const FROG::Vector<FROG::StringView> &arguments);
  void KeyEventCallback(Keyboard::KeyEvent);

private:
  FROG::String m_buffer;
};

} // namespace Kernel