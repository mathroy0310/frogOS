/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Queue.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:16:46 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 14:28:26 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Errors.h>
#include <FROG/Math.h>
#include <FROG/Memory.h>
#include <FROG/Move.h>

namespace FROG {

template <typename T> class Queue {
  public:
	using size_type = size_t;
	using value_type = T;

  public:
	Queue() = default;
	Queue(Queue<T> &&);
	Queue(const Queue<T> &);
	~Queue();

	Queue<T> &operator=(Queue<T> &&);
	Queue<T> &operator=(const Queue<T> &);

	[[nodiscard]] ErrorOr<void>                             Push(T &&);
	[[nodiscard]] ErrorOr<void>                             Push(const T &);
	template <typename... Args> [[nodiscard]] ErrorOr<void> Emplace(Args...);

	void Pop();
	void Clear();

	bool      Empty() const;
	size_type Size() const;

	const T &Front() const;
	T       &Front();

  private:
	[[nodiscard]] ErrorOr<void> EnsureCapacity(size_type size);
	const T                    *AddressOf(size_type, void* = nullptr) const;
	T                          *AddressOf(size_type, void* = nullptr);

  private:
	uint8_t  *m_data = nullptr;
	size_type m_capacity = 0;
	size_type m_size = 0;
};

template <typename T> Queue<T>::Queue(Queue<T> &&other) {
	m_data = other.m_data;
	m_capacity = other.m_capacity;
	m_size = other.m_size;

	other.m_data = nullptr;
	other.m_capacity = 0;
	other.m_size = 0;
}

template <typename T> Queue<T>::Queue(const Queue<T> &other) {
	MUST(EnsureCapacity(other.Size()));
	for (size_type i = 0; i < other.Size(); i++)
		new (AddressOf(i)) T(*AddressOf(i, other.m_data));
	m_size = other.m_size;
}

template <typename T> Queue<T>::~Queue() { Clear(); }

template <typename T> Queue<T> &Queue<T>::operator=(Queue<T> &&other) {
	Clear();

	m_data = other.m_data;
	m_capacity = other.m_capacity;
	m_size = other.m_size;

	other.m_data = nullptr;
	other.m_capacity = 0;
	other.m_size = 0;

	return *this;
}

template <typename T> Queue<T> &Queue<T>::operator=(const Queue<T> &other) {
	Clear();
	MUST(EnsureCapacity(other.Size()));
	for (size_type i = 0; i < other.Size(); i++)
		new (AddressOf(i)) T(*AddressOf(i, other.m_data));
	m_size = other.m_size;
	return *this;
}

template <typename T> ErrorOr<void> Queue<T>::Push(T &&value) {
	TRY(EnsureCapacity(m_size + 1));
	new (AddressOf(m_size)) T(Move(value));
	m_size++;
	return {};
}

template <typename T> ErrorOr<void> Queue<T>::Push(const T &value) {
	return Push(Move(T(value)));
}

template <typename T>
template <typename... Args>
ErrorOr<void> Queue<T>::Emplace(Args... args) {
	TRY(EnsureCapacity(m_size + 1));
	new (AddressOf(m_size)) T(Forward<Args>(args)...);
	m_size++;
	return {};
}

template <typename T> void Queue<T>::Pop() {
	ASSERT(m_size > 0);
	for (size_type i = 0; i < m_size - 1; i++)
		*AddressOf(i) = Move(*AddressOf(i + 1));
	AddressOf(m_size - 1)->~T();
	m_size--;
}

template <typename T> void Queue<T>::Clear() {
	for (size_type i = 0; i < m_size; i++)
		AddressOf(i)->~T();
	FROG::deallocator(m_data);
	m_data = nullptr;
	m_capacity = 0;
	m_size = 0;
}

template <typename T> bool Queue<T>::Empty() const { return m_size == 0; }

template <typename T> typename Queue<T>::size_type Queue<T>::Size() const {
	return m_size;
}

template <typename T> const T &Queue<T>::Front() const {
	ASSERT(m_size > 0);
	return *AddressOf(0);
}

template <typename T> T &Queue<T>::Front() {
	ASSERT(m_size > 0);
	return *AddressOf(0);
}

template <typename T> ErrorOr<void> Queue<T>::EnsureCapacity(size_type size) {
	if (m_capacity > size) return {};
	size_type new_cap = FROG::Math::max<size_type>(size, m_capacity * 3 / 2);
	uint8_t  *new_data = (uint8_t *) FROG::allocator(new_cap * sizeof(T));
	if (new_data == nullptr)
		return Error::FromString("Queue: Could not allocate memory");
	for (size_type i = 0; i < m_size; i++) {
		new (AddressOf(i, new_data)) T(Move(*AddressOf(i)));
		AddressOf(i)->~T();
	}
	FROG::deallocator(m_data);
	m_data = new_data;
	m_capacity = new_cap;
	return {};
}

template <typename T>
const T *Queue<T>::AddressOf(size_type index, void *base) const {
	if (base == nullptr) base = m_data;
	return (T *) base + index;
}

template <typename T> T *Queue<T>::AddressOf(size_type index, void *base) {
	if (base == nullptr) base = m_data;
	return (T *) base + index;
}

} // namespace FROG