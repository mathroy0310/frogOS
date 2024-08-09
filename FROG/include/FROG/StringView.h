/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   StringView.h                                 ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:17:09 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 02:09:53 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Formatter.h>
#include <FROG/Forward.h>

namespace FROG {

class StringView {
  public:
	using size_type = size_t;

  public:
	StringView();
	StringView(const String &);
	StringView(const char *, size_type = -1);

	char operator[](size_type) const;

	bool operator==(const String &) const;
	bool operator==(StringView) const;
	bool operator==(const char *) const;

	StringView Substring(size_type, size_type = -1) const;

	ErrorOr<Vector<StringView>> Split(char, bool = false);
	ErrorOr<Vector<StringView>> Split(bool (*comp)(char), bool = false);
	
	size_type Count(char) const;

	bool      Empty() const;
	size_type Size() const;

	const char *Data() const;

  private:
	const char *m_data = nullptr;
	size_type   m_size = 0;
};

} // namespace FROG

namespace FROG::Formatter {

template <void (*PUTC_LIKE)(char)>
void print_argument_impl(const StringView &sv, const ValueFormat &) {
	for (StringView::size_type i = 0; i < sv.Size(); i++)
		PUTC_LIKE(sv[i]);
}

} // namespace FROG::Formatter
