/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Vector.h                                     ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:17:04 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/05 01:17:05 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Errors.h>
#include <FROG/Memory.h>

#include <assert.h>
#include <string.h>
#include <sys/param.h>

namespace FROG {

template <typename T> class Vector {
  public:
	using size_type = size_t;
	using value_type = T;

  public:
	Vector() = default;
	~Vector();

	ErrorOr<void> PushBack(const T &);
	ErrorOr<void> Insert(const T &, size_type);

	void PopBack();
	void Remove(size_type);

	const T &operator[](size_type) const;
	T &operator[](size_type);

	const T &Back() const;
	T       &Back();
	const T &Front() const;
	T       &Front();

	ErrorOr<void> Resize(size_type);
	ErrorOr<void> Reserve(size_type);

	bool      Empty() const;
	size_type Size() const;
	size_type Capasity() const;

  private:
	ErrorOr<void> EnsureCapasity(size_type);

  private:
	T        *m_data = nullptr;
	size_type m_capasity = 0;
	size_type m_size = 0;
};

template <typename T> Vector<T>::~Vector() {
	for (size_type i = 0; i < m_size; i++)
		m_data[i].~T();
	FROG::deallocator(m_data);
}

template <typename T> ErrorOr<void> Vector<T>::PushBack(const T &value) {
	TRY(EnsureCapasity(m_size + 1));
	m_data[m_size] = value;
	m_size++;
	return {};
}

template <typename T>
ErrorOr<void> Vector<T>::Insert(const T &value, size_type index) {
	assert(index <= m_size);
	TRY(EnsureCapasity(m_size + 1));
	memmove(m_data + index + 1, m_data + index, (m_size - index) * sizeof(T));
	m_data[index] = value;
	m_size++;
	return {};
}

template <typename T> void Vector<T>::PopBack() {
	assert(m_size > 0);
	m_data[m_size - 1].~T();
	m_size--;
}

template <typename T> void Vector<T>::Remove(size_type index) {
	assert(index < m_size);
	m_data[index].~T();
	memmove(m_data + index, m_data + index + 1, (m_size - index - 1) * sizeof(T));
	m_size--;
}

template <typename T> const T &Vector<T>::operator[](size_type index) const {
	assert(index < m_size);
	return m_data[index];
}

template <typename T> T &Vector<T>::operator[](size_type index) {
	assert(index < m_size);
	return m_data[index];
}

template <typename T> const T &Vector<T>::Back() const {
	assert(m_size > 0);
	return m_data[m_size - 1];
}

template <typename T> T &Vector<T>::Back() {
	assert(m_size > 0);
	return m_data[m_size - 1];
}

template <typename T> const T &Vector<T>::Front() const {
	assert(m_size > 0);
	return m_data[0];
}
template <typename T> T &Vector<T>::Front() {
	assert(m_size > 0);
	return m_data[0];
}

template <typename T> ErrorOr<void> Vector<T>::Resize(size_type size) {
	if (size < m_size) {
		for (size_type i = size; i < m_size; i++)
			m_data[i].~T();
		m_size = size;
	} else if (size > m_size) {
		TRY(EnsureCapasity(size));
		for (size_type i = m_size; i < size; i++)
			m_data[i] = T();
		m_size = size;
	}
	m_size = size;
	return {};
}

template <typename T> ErrorOr<void> Vector<T>::Reserve(size_type size) {
	TRY(EnsureCapasity(size));
	return {};
}

template <typename T> bool Vector<T>::Empty() const {
	return m_size == 0;
}

template <typename T> typename Vector<T>::size_type Vector<T>::Size() const {
	return m_size;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::Capasity() const {
	return m_capasity;
}

template <typename T> ErrorOr<void> Vector<T>::EnsureCapasity(size_type size) {
	if (m_capasity >= size)
		return {};
	size_type new_cap = MAX(size, m_capasity * 1.5f);
	void     *new_data = FROG::allocator(new_cap * sizeof(T));
	if (new_data == nullptr)
		return Error::FromString("Vector: Could not allocate memory");
	memcpy(new_data, m_data, m_size * sizeof(T));
	FROG::deallocator(m_data);
	m_data = (T *) new_data;
	m_capasity = new_cap;
	return {};
}

} // namespace FROG