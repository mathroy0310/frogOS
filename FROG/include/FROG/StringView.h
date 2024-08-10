/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   StringView.h                                 ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:17:09 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 13:36:14 by mathroy0310    `                         */
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

	[[nodiscard]] ErrorOr<Vector<StringView>> Split(char, bool = false);
	[[nodiscard]] ErrorOr<Vector<StringView>> Split(bool (*comp)(char), bool = false);

	char Back() const;
	char Front() const;

	size_type Count(char) const;

	bool      Empty() const;
	size_type Size() const;

	const char *Data() const;

  private:
	const char *m_data = nullptr;
	size_type   m_size = 0;
};

} // namespace FROG

inline FROG::StringView operator""_sv(const char *str, FROG::StringView::size_type len) {
	return FROG::StringView(str, len);
}

namespace FROG::Formatter {

template <typename F>
void print_argument_impl(F putc, const StringView &sv, const ValueFormat &) {
	for (StringView::size_type i = 0; i < sv.Size(); i++)
		putc(sv[i]);
}

} // namespace FROG::Formatter