/*
* handy.h
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

/* INFO:
*  Bunch of uncategorized utils
*/

#pragma once

#include <type_traits>
#include <vector>
#include <array>
#include <cassert>

#include "template_math.h"

namespace kmu
{
	namespace impl // Implementation
	{
		template<typename Signature> // GCC's implementation
		struct x_result_of;

		template<typename Functor, typename... ArgTypes>
		struct x_result_of<Functor( ArgTypes... )>
		{
			typedef decltype( std::declval<Functor>()( std::declval<ArgTypes>()... ) ) type;
		};

		template<typename T>
		using x_result_of_t = typename x_result_of<T>::type;
	} // namespace impl

	template<typename First, typename... Rest>
	struct is_one_of;

	template<typename First, typename Second, typename... Rest>
	struct is_one_of<First, Second, Rest...>
	{
		static const bool value = std::is_same<First, Second>::value
				? true : kmu::is_one_of<First, Rest...>::value;
	};

	template<typename First, typename Second>
	struct is_one_of<First, Second>
	{
		static const bool value = std::is_same<First, Second>::value;
	};

	template<typename Only>
	struct is_one_of<Only>
	{
		static const bool value = false;
	};

	template<typename... Args>
	struct are_all_unique;

	template<typename First, typename... Rest>
	struct are_all_unique<First, Rest...>
	{
		static const bool value = !kmu::is_one_of<First, Rest...>::value
									&& kmu::are_all_unique<Rest...>::value;
	};

	template<typename Only>
	struct are_all_unique<Only>
	{
		static const bool value = true;
	};

	template<typename T>
	std::vector<T> makeVectorOfElements( const T& element, size_t count = 1 )
	{
		return std::vector<T>( count, element );
	}

	template<int From, int To,
		size_t Count = ( size_t ) kmu::distance<From, To>::value + 1>
	inline const std::array<int, Count> makeRange()
	{
		std::array<int, Count> tab;

		int step = (To - From) < 0 ? -1 : 1;
		int startValue = From;
		for( int i = 0; i < Count; ++i )
		{
			tab[i] = startValue;
			startValue += step;
		}

		return tab;
	}
} // namespace kmu