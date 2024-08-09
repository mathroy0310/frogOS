/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   Array,h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/09 11:24:17 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 11:24:28 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <assert.h>
#include <stddef.h>

namespace FROG
{

	template<typename T, size_t S>
	class Array
	{
	public:
		using size_type = decltype(S);
		using value_type = T;

	public:
		Array();
		Array(const T&);

		const T& operator[](size_type) const;
		T& operator[](size_type);

		const T& Back() const;
		T& Back();
		const T& Front() const;
		T& Front();

		constexpr size_type Size() const;

	private:
		T m_data[S];
	};

	template<typename T, size_t S>
	Array<T, S>::Array()
	{
		for (size_type i = 0; i < S; i++)
			m_data[i] = T();
	}

	template<typename T, size_t S>
	Array<T, S>::Array(const T& value)
	{
		for (size_type i = 0; i < S; i++)
			m_data[i] = value;
	}

	template<typename T, size_t S>
	const T& Array<T, S>::operator[](size_type index) const
	{
		ASSERT(index < S);
		return m_data[index];
	}

	template<typename T, size_t S>
	T& Array<T, S>::operator[](size_type index)
	{
		ASSERT(index < S);
		return m_data[index];
	}

	template<typename T, size_t S>
	const T& Array<T, S>::Back() const
	{
		ASSERT(S != 0);
		return m_data[S - 1];
	}

	template<typename T, size_t S>
	T& Array<T, S>::Back()
	{
		ASSERT(S != 0);
		return m_data[S - 1];
	}

	template<typename T, size_t S>
	const T& Array<T, S>::Front() const
	{
		ASSERT(S != 0);
		return m_data[0];
	}

	template<typename T, size_t S>
	T& Array<T, S>::Front()
	{
		ASSERT(S != 0);
		return m_data[0];
	}

	template<typename T, size_t S>
	constexpr typename Array<T, S>::size_type Array<T, S>::Size() const
	{
		return S;
	}

}