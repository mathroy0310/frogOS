/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   String.cpp                                   ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:16:29 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/12 02:48:34 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Memory.h>
#include <FROG/Errors.h>
#include <FROG/Math.h>
#include <FROG/Move.h>
#include <FROG/String.h>
#include <FROG/StringView.h>

#include <string.h>

namespace FROG {

String::String() { MUST(copy_impl(""_sv)); }

String::String(const String &other) { MUST(copy_impl(other.SV())); }

String::String(String &&other) { move_impl(Move(other)); }

String::String(StringView other) { MUST(copy_impl(other)); }

String::~String() { FROG::deallocator(m_data); }

String &String::operator=(const String &other) {
	MUST(copy_impl(other.SV()));
	return *this;
}

String &String::operator=(String &&other) {
	FROG::deallocator(m_data);
	move_impl(Move(other));
	return *this;
}

String &String::operator=(StringView other) {
	MUST(copy_impl(other));
	return *this;
}

ErrorOr<void> String::PushBack(char ch) {
	TRY(EnsureCapasity(m_size + 2));
	m_data[m_size] = ch;
	m_size++;
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::Insert(char ch, size_type index) {
	ASSERT(index <= m_size);
	TRY(EnsureCapasity(m_size + 1 + 1));
	memmove(m_data + index + 1, m_data + index, m_size - index);
	m_data[index] = ch;
	m_size += 1;
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::Insert(StringView other, size_type index) {
	dprintln("insert '{}' to '{}' at index {}", other, *this, index);

	ASSERT(index <= m_size);
	TRY(EnsureCapasity(m_size + other.Size() + 1));
	memmove(m_data + index + other.Size(), m_data + index, m_size - index);
	memcpy(m_data + index, other.Data(), other.Size());
	m_size += other.Size();
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::Append(StringView other) {
	TRY(EnsureCapasity(m_size + other.Size() + 1));
	memcpy(m_data + m_size, other.Data(), other.Size());
	m_size += other.Size();
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::Append(const String &string) {
	TRY(Append(string.SV()));
	return {};
}

void String::PopBack() {
	ASSERT(m_size > 0);
	m_size--;
	m_data[m_size] = '\0';
}

void String::Remove(size_type index) { Erase(index, 1); }

void String::Erase(size_type index, size_type count) {
	ASSERT(index + count <= m_size);
	memmove(m_data + index, m_data + index + count, m_size - index - count);
	m_size -= count;
	m_data[m_size] = '\0';
}

void String::Clear() {
	m_size = 0;
	m_data[0] = '\0';
}

char String::operator[](size_type index) const {
	ASSERT(index < m_size);
	return m_data[index];
}

char &String::operator[](size_type index) {
	ASSERT(index < m_size);
	return m_data[index];
}

bool String::operator==(const String &other) const {
	if (m_size != other.m_size) return false;
	return memcmp(m_data, other.m_data, m_size) == 0;
}

bool String::operator==(StringView other) const {
	if (m_size != other.Size()) return false;
	return memcmp(m_data, other.Data(), m_size) == 0;
}

bool String::operator==(const char *other) const {
	for (size_type i = 0; i <= m_size; i++)
		if (m_data[i] != other[i]) return false;
	return true;
}

ErrorOr<void> String::Resize(size_type size, char ch) {
	if (size < m_size) {
		m_data[size] = '\0';
		m_size = size;
	} else if (size > m_size) {
		TRY(EnsureCapasity(size + 1));
		for (size_type i = m_size; i < size; i++)
			m_data[i] = ch;
		m_data[size] = '\0';
		m_size = size;
	}
	m_size = size;
	return {};
}

ErrorOr<void> String::Reserve(size_type size) {
	TRY(EnsureCapasity(size));
	return {};
}

StringView String::SV() const { return StringView(*this); }

bool String::Empty() const { return m_size == 0; }

String::size_type String::Size() const { return m_size; }

String::size_type String::Capasity() const { return m_capasity; }

const char *String::Data() const { return m_data; }

ErrorOr<void> String::EnsureCapasity(size_type size) {
	if (m_capasity >= size) return {};
	size_type new_cap = FROG::Math::max<size_type>(size, m_capasity * 3 / 2);
	void     *new_data = FROG::allocator(new_cap);
	if (new_data == nullptr)
		return Error::FromString("String: Could not allocate memory");
	if (m_data) memcpy(new_data, m_data, m_size + 1);
	FROG::deallocator(m_data);
	m_data = (char *) new_data;
	m_capasity = new_cap;
	return {};
}

ErrorOr<void> String::copy_impl(StringView other) {
	TRY(EnsureCapasity(other.Size() + 1));
	memcpy(m_data, other.Data(), other.Size());
	m_size = other.Size();
	m_data[m_size] = '\0';
	return {};
}

void String::move_impl(String &&other) {
	m_data = other.m_data;
	m_size = other.m_size;
	m_capasity = other.m_capasity;

	other.m_data = nullptr;
	other.m_size = 0;
	other.m_capasity = 0;
}

} // namespace FROG