/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   StringView.cpp                               ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:16:32 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 02:20:00 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/String.h>
#include <FROG/StringView.h>
#include <FROG/Vector.h>

#include <assert.h>

namespace FROG {

StringView::StringView() {
}

StringView::StringView(const String &other)
    : StringView(other.Data(), other.Size()) {
}

StringView::StringView(const char *string, size_type len) {
	if (len == size_type(-1))
		len = strlen(string);
	m_data = string;
	m_size = len;
}

char StringView::operator[](size_type index) const {
	assert(index < m_size);
	return m_data[index];
}

bool StringView::operator==(const String &other) const {
	if (m_size != other.Size())
		return false;
	return memcmp(m_data, other.Data(), m_size) == 0;
}

bool StringView::operator==(StringView other) const {
	if (m_size != other.m_size)
		return false;
	return memcmp(m_data, other.m_data, m_size) == 0;
}

bool StringView::operator==(const char *other) const {
	if (memcmp(m_data, other, m_size))
		return false;
	return other[m_size] == '\0';
}

StringView StringView::Substring(size_type index, size_type len) const {
	assert(index <= m_size);
	if (len == size_type(-1))
		len = m_size - index;
	assert(len <= m_size - index); // weird order to avoid overflow
	StringView result;
	result.m_data = m_data + index;
	result.m_size = len;
	return result;
}

ErrorOr<Vector<StringView>> StringView::Split(char delim, bool allow_empties) {
	// FIXME: Won't work while multithreading
	static char s_delim = delim;
	return Split([](char c) { return c == s_delim; }, allow_empties);
}

ErrorOr<Vector<StringView>> StringView::Split(bool (*comp)(char), bool allow_empties) {
	size_type count = 0;
	{
		size_type start = 0;
		for (size_type i = 0; i < m_size; i++) {
			if (comp(m_data[i])) {
				if (allow_empties || start != i)
					count++;
				start = i + 1;
			}
		}
		if (start != m_size)
			count++;
	}

	Vector<StringView> result;
	TRY(result.Reserve(count));

	size_type start = 0;
	for (size_type i = 0; i < m_size; i++) {
		if (comp(m_data[i])) {
			if (allow_empties || start != i)
				result.PushBack(this->Substring(start, i - start));
			start = i + 1;
		}
	}
	if (start != m_size)
		result.PushBack(this->Substring(start));
	return result;
}

StringView::size_type StringView::Count(char ch) const {
	size_type result = 0;
	for (size_type i = 0; i < m_size; i++)
		if (m_data[i] == ch)
			result++;
	return result;
}

bool StringView::Empty() const {
	return m_size == 0;
}

StringView::size_type StringView::Size() const {
	return m_size;
}

const char *StringView::Data() const {
	return m_data;
}

} // namespace FROG