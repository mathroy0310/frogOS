/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Iterators.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 23:47:51 by maroy             #+#    #+#             */
/*   Updated: 2024/09/10 23:53:19 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Assert.h>

namespace FROG {
template <typename T, typename Container> class IteratorSimple {
  public:
	IteratorSimple() = default;

	const T &operator*() const {
		ASSERT(m_pointer);
		return *m_pointer;
	}

	T &operator*() {
		ASSERT(m_pointer);
		return *m_pointer;
	}

	const T *operator->() const {
		ASSERT(m_pointer);
		return m_pointer;
	}

	T *operator->() {
		ASSERT(m_pointer);
		return m_pointer;
	}

	IteratorSimple &operator++() {
		ASSERT(m_pointer);
		++m_pointer;
		return *this;
	}

	IteratorSimple operator++(int) {
		auto tmp = *this;
		++(*this);
		return tmp;
	}

	IteratorSimple &operator--() {
		ASSERT(m_pointer);
		return --m_pointer;
	}

	IteratorSimple operator--(int) {
		auto tmp = *this;
		--(*this);
		return tmp;
	}

	bool operator==(const IteratorSimple &other) const { return m_pointer == other.m_pointer; }

	operator bool() const { return m_pointer; }

  private:
	IteratorSimple(T *pointer) : m_pointer(pointer) {}

  private:
	T *m_pointer = nullptr;
	friend Container;
};

template <typename T, typename Container> class ConstIteratorSimple {
  public:
	ConstIteratorSimple() = default;
	ConstIteratorSimple(IteratorSimple<T, Container> other) : m_pointer(other.m_pointer) {}

	const T &operator*() const {
		ASSERT(m_pointer);
		return *m_pointer;
	}

	const T *operator->() const {
		ASSERT(m_pointer);
		return m_pointer;
	}

	ConstIteratorSimple &operator++() {
		ASSERT(m_pointer);
		++m_pointer;
		return *this;
	}
	ConstIteratorSimple operator++(int) {
		auto temp = *this;
		++(*this);
		return temp;
	}

	ConstIteratorSimple &operator--() {
		ASSERT(m_pointer);
		return --m_pointer;
	}
	ConstIteratorSimple operator--(int) {
		auto temp = *this;
		--(*this);
		return temp;
	}

	bool operator==(const ConstIteratorSimple &other) const { return m_pointer == other.m_pointer; }
	bool operator!=(const ConstIteratorSimple &other) const { return !(*this == other); }

	operator bool() const { return !!m_pointer; }

  private:
	ConstIteratorSimple(const T *pointer) : m_pointer(pointer) {}

  private:
	const T *m_pointer = nullptr;

	friend Container;
};
} // namespace FROG