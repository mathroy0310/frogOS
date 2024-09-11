/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringView.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 01:17:09 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/11 00:00:24 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Formatter.h>
#include <FROG/ForwardList.h>
#include <FROG/Iterators.h>

namespace FROG {

class StringView {
  public:
	using size_type = size_t;
	using const_iterator = ConstIteratorSimple<char, StringView>;

  public:
	StringView();
	StringView(const String &);
	StringView(const char *, size_type = -1);

	const_iterator begin() const { return const_iterator(m_data); }
	const_iterator end() const { return const_iterator(m_data + m_size); }

	char operator[](size_type) const;

	bool operator==(const String &) const;
	bool operator==(StringView) const;
	bool operator==(const char *) const;

	StringView substring(size_type, size_type = -1) const;

	ErrorOr<Vector<StringView>> split(char, bool = false);
	ErrorOr<Vector<StringView>> split(bool (*comp)(char), bool = false);

	char back() const;
	char front() const;

	size_type count(char) const;

	bool      empty() const;
	size_type size() const;

	const char *data() const;

  private:
	const char *m_data = nullptr;
	size_type   m_size = 0;
};

} // namespace FROG

inline FROG::StringView operator""sv(const char *str, FROG::StringView::size_type len) {
	return FROG::StringView(str, len);
}

namespace FROG::Formatter {

template <typename F> void print_argument(F putc, const StringView &sv, const ValueFormat &) {
	for (StringView::size_type i = 0; i < sv.size(); i++)
		putc(sv[i]);
}

} // namespace FROG::Formatter