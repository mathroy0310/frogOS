/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Queue.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 20:14:45 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 20:44:36 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Errors.h>
#include <kernel/kmalloc.h>

#if defined(__is_kernel)
#include <kernel/kmalloc.h>
#else
#include <stdlib.h>
#endif

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <sys/param.h>

namespace FROG {

template <typename T> class Queue {
  private:
#if defined(__is_kernel)
	static constexpr auto &allocator = kmalloc;
	static constexpr auto &deallocator = kfree;
#else
	static constexpr auto &allocator = malloc;
	static constexpr auto &deallocator = free;
#endif

  public:
	using size_type = uint32_t;
	using value_type = T;

  public:
	Queue() = default;
	~Queue();

	ErrorOr<void> Push(const T &value);
	void          Pop();

	bool      Empty() const;
	size_type Size() const;

	const T &Front() const;
	T       &Front();

  private:
	ErrorOr<void> VerifyCapacity(size_type size);

  private:
	T        *m_data = nullptr;
	size_type m_capacity = 0;
	size_type m_size = 0;
};

template <typename T> Queue<T>::~Queue() {
	Queue<T>::deallocator(m_data);
}

template <typename T> ErrorOr<void> Queue<T>::Push(const T &value) {
	TRY(VerifyCapacity(m_size + 1));
	m_data[m_size++] = value;
	return {};
}

template <typename T> void Queue<T>::Pop() {
	assert(m_size > 0);
	m_data->~T();
	memmove(m_data, m_data + 1, sizeof(T) * (--m_size));
}

template <typename T> bool Queue<T>::Empty() const {
	return m_size == 0;
}

template <typename T> typename Queue<T>::size_type Queue<T>::Size() const {
	return m_size;
}

template <typename T> const T &Queue<T>::Front() const {
	assert(m_size > 0);
	return *m_data;
}

template <typename T> T &Queue<T>::Front() {
	assert(m_size > 0);
	return *m_data;
}

template <typename T> ErrorOr<void> Queue<T>::VerifyCapacity(size_type size) {
	if (m_capacity > size)
		return {};

	size_type new_cap = MAX(m_capacity * 1.5f, m_capacity + 1);
	void     *new_data = Queue<T>::allocator(new_cap * sizeof(T));
	if (new_data == nullptr)
		return Error::FromString("Queue: out of memory");

	memcpy(new_data, m_data, m_size * sizeof(T));
	Queue<T>::deallocator(m_data);

	m_data = (T *) new_data;
	m_capacity = new_cap;

	return {};
}

} // namespace FROG