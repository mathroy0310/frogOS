/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CircularQueue.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:26:01 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 15:26:41 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Assert.h>
#include <stddef.h>
#include <stdint.h>

namespace FROG {

template <typename T, size_t S> class CircularQueue {
  public:
	using size_type = size_t;
	using value_type = T;

  public:
	CircularQueue() = default;
	~CircularQueue();
	void                             push(const T &);
	void                             push(T &&);
	template <typename... Args> void emplace(Args &&...args);
	void                             pop();
	const T                         &front() const;
	T                               &front();
	size_type                        size() const { return m_size; }
	bool                             empty() const { return size() == 0; }
	bool                             full() const { return size() == capacity(); }

	static constexpr size_type capacity() { return S; }

  private:
	T       *element_at(size_type);
	const T *element_at(size_type) const;

  private:
	alignas(T) uint8_t m_storage[sizeof(T) * capacity()];
	size_type m_first{0};
	size_type m_size{0};
};

template <typename T, size_t S> CircularQueue<T, S>::~CircularQueue() {
	for (size_type i = 0; i < m_size; i++)
		element_at((m_first + i) % capacity())->~T();
}

template <typename T, size_t S> void CircularQueue<T, S>::push(const T &value) {
	emplace(FROG::move(T(value)));
}

template <typename T, size_t S> void CircularQueue<T, S>::push(T &&value) {
	emplace(FROG::move(value));
}

template <typename T, size_t S>
template <typename... Args>
void CircularQueue<T, S>::emplace(Args &&...args) {
	ASSERT(!full());
	new (element_at(((m_first + m_size) % capacity()))) T(FROG::forward<Args>(args)...);
	m_size++;
}

template <typename T, size_t S> void CircularQueue<T, S>::pop() {
	ASSERT(!empty());
	element_at(m_first)->~T();
	m_first = (m_first + 1) % capacity();
	m_size--;
}

template <typename T, size_t S> const T &CircularQueue<T, S>::front() const {
	ASSERT(!empty());
	return *element_at(m_first);
}

template <typename T, size_t S> T &CircularQueue<T, S>::front() {
	ASSERT(!empty());
	return *element_at(m_first);
}

template <typename T, size_t S> const T *CircularQueue<T, S>::element_at(size_type index) const {
	ASSERT(index < capacity());
	return (const T *) (m_storage + index * sizeof(T));
}

template <typename T, size_t S> T *CircularQueue<T, S>::element_at(size_type index) {
	ASSERT(index < capacity());
	return (T *) (m_storage + index * sizeof(T));
}
} // namespace FROG