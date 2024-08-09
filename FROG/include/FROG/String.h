/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   String.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:16:49 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 09:09:16 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Errors.h>
#include <FROG/Formatter.h>
#include <FROG/Forward.h>

namespace FROG {

class String {
  public:
	using size_type = size_t;

  public:
	String();
	String(const String &);
	String(String &&);
	String(const StringView &);
	String(const char *, size_type = -1);
	~String();

	template <typename... Args>
	static String Formatted(const char *format, const Args &...args);

	String &operator=(const String &);
	String &operator=(String &&);

	ErrorOr<void> PushBack(char);
	ErrorOr<void> Insert(char, size_type);
	ErrorOr<void> Append(const char *);
	ErrorOr<void> Append(const String &);

	void PopBack();
	void Remove(size_type);

	void Clear();

	char operator[](size_type) const;
	char &operator[](size_type);

	bool operator==(const String &) const;
	bool operator==(StringView) const;
	bool operator==(const char *) const;

	ErrorOr<void> Resize(size_type, char = '\0');
	ErrorOr<void> Reserve(size_type);

	StringView SV() const;

	bool      Empty() const;
	size_type Size() const;
	size_type Capasity() const;

	const char *Data() const;

  private:
	ErrorOr<void> EnsureCapasity(size_type);

	ErrorOr<void> copy_impl(const char *, size_type);
	void          move_impl(String &&);

  private:
	char     *m_data = nullptr;
	size_type m_capasity = 0;
	size_type m_size = 0;
};

template <typename... Args>
String String::Formatted(const char *format, const Args &...args) {
	String result;
	FROG::Formatter::print([&](char c) { result.PushBack(c); }, format, args...);
	return result;
}

} // namespace FROG

namespace FROG::Formatter {

template <typename F>
void print_argument_impl(F putc, const String &string, const ValueFormat &) {
	for (String::size_type i = 0; i < string.Size(); i++)
		putc(string[i]);
}

} // namespace FROG::Formatter