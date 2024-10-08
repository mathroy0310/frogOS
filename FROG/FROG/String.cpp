/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   String.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 01:16:29 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/09/03 13:57:50 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <FROG/Math.h>
#include <FROG/Memory.h>
#include <FROG/Move.h>
#include <FROG/String.h>
#include <FROG/StringView.h>

#include <string.h>

namespace FROG {

String::String() { MUST(copy_impl(""sv)); }

String::String(const String &other) { MUST(copy_impl(other.sv())); }

String::String(String &&other) { move_impl(move(other)); }

String::String(StringView other) { MUST(copy_impl(other)); }

String::~String() { FROG::deallocator(m_data); }

String &String::operator=(const String &other) {
	MUST(copy_impl(other.sv()));
	return *this;
}

String &String::operator=(String &&other) {
	FROG::deallocator(m_data);
	move_impl(move(other));
	return *this;
}

String &String::operator=(StringView other) {
	MUST(copy_impl(other));
	return *this;
}

ErrorOr<void> String::push_back(char ch) {
	TRY(ensure_capacity(m_size + 2));
	m_data[m_size] = ch;
	m_size++;
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::insert(char ch, size_type index) {
	ASSERT(index <= m_size);
	TRY(ensure_capacity(m_size + 1 + 1));
	memmove(m_data + index + 1, m_data + index, m_size - index);
	m_data[index] = ch;
	m_size += 1;
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::insert(StringView other, size_type index) {
	// dprintln("insert '{}' to '{}' at index {}", other, *this, index);

	ASSERT(index <= m_size);
	TRY(ensure_capacity(m_size + other.size() + 1));
	memmove(m_data + index + other.size(), m_data + index, m_size - index);
	memcpy(m_data + index, other.data(), other.size());
	m_size += other.size();
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::append(StringView other) {
	TRY(ensure_capacity(m_size + other.size() + 1));
	memcpy(m_data + m_size, other.data(), other.size());
	m_size += other.size();
	m_data[m_size] = '\0';
	return {};
}

ErrorOr<void> String::append(const String &string) {
	TRY(append(string.sv()));
	return {};
}

void String::pop_back() {
	ASSERT(m_size > 0);
	m_size--;
	m_data[m_size] = '\0';
}

void String::remove(size_type index) { erase(index, 1); }

void String::erase(size_type index, size_type count) {
	ASSERT(index + count <= m_size);
	memmove(m_data + index, m_data + index + count, m_size - index - count);
	m_size -= count;
	m_data[m_size] = '\0';
}

void String::clear() {
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
	if (m_size != other.size()) return false;
	return memcmp(m_data, other.data(), m_size) == 0;
}

bool String::operator==(const char *other) const {
	for (size_type i = 0; i <= m_size; i++)
		if (m_data[i] != other[i]) return false;
	return true;
}

ErrorOr<void> String::resize(size_type size, char ch) {
	if (size < m_size) {
		m_data[size] = '\0';
		m_size = size;
	} else if (size > m_size) {
		TRY(ensure_capacity(size + 1));
		for (size_type i = m_size; i < size; i++)
			m_data[i] = ch;
		m_data[size] = '\0';
		m_size = size;
	}
	m_size = size;
	return {};
}

ErrorOr<void> String::reserve(size_type size) {
	TRY(ensure_capacity(size));
	return {};
}

ErrorOr<void> String::shrink_to_fit() {
	size_type temp = m_capacity;
	m_capacity = 0;
	auto error_or = ensure_capacity(m_size);
	if (error_or.is_error()) {
		m_capacity = temp;
		return error_or;
	}
	return {};
}

StringView String::sv() const { return StringView(*this); }

bool String::empty() const { return m_size == 0; }

String::size_type String::size() const { return m_size; }

String::size_type String::capacity() const { return m_capacity; }

const char *String::data() const { return m_data; }

ErrorOr<void> String::ensure_capacity(size_type size) {
	if (m_capacity >= size) return {};
	size_type new_cap = FROG::Math::max<size_type>(size, m_capacity * 2);
	void     *new_data = FROG::allocator(new_cap);
	if (new_data == nullptr) return Error::from_errno(ENOMEM);
	if (m_data) memcpy(new_data, m_data, m_size + 1);
	FROG::deallocator(m_data);
	m_data = (char *) new_data;
	m_capacity = new_cap;
	return {};
}

ErrorOr<void> String::copy_impl(StringView other) {
	TRY(ensure_capacity(other.size() + 1));
	memcpy(m_data, other.data(), other.size());
	m_size = other.size();
	m_data[m_size] = '\0';
	return {};
}

void String::move_impl(String &&other) {
	m_data = other.m_data;
	m_size = other.m_size;
	m_capacity = other.m_capacity;

	other.m_data = nullptr;
	other.m_size = 0;
	other.m_capacity = 0;
}

} // namespace FROG