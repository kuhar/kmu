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

namespace kmu
{

	namespace impl // Implementation
	{
		template <size_t... Index>
		struct x_index_tuple_type
		{
			template<size_t N>
			using append = x_index_tuple_type<Index..., N>;
		};

		template<size_t N>
		struct x_make_index
		{
			using type = typename x_make_index<N - 1>::type::template append<N - 1>;
		};

		template<>
		struct x_make_index<0>
		{
			using type = x_index_tuple_type<>;
		};

		template<size_t N>
		using x_index_tuple = typename x_make_index<N>::type;

		template<typename Signature> // GCC's implementation
		class x_result_of;

		template<typename Functor, typename... ArgTypes>
		struct x_result_of<Functor( ArgTypes... )>
		{
			typedef decltype( std::declval<Functor>()( std::declval<ArgTypes>()... ) ) type;
		};

		template<int FirstValue, int SecondValue>
		struct x_absolute_value
		{
			enum
			{
				value = ( FirstValue - SecondValue ) < 0 ? 
				-( FirstValue - SecondValue ) : ( FirstValue - SecondValue )
			};
		};

		template<int Value>
		struct x_absolute_value<Value, 0>
		{
			enum
			{
				value = Value < 0 ? -Value : Value
			};
		};

	} // namespace impl

	template<typename T>
	std::vector<T> makeVectorOfElements( const T& element, size_t count = 1 )
	{
		return std::vector<T>( count, element );
	}

	template<int From, int To,
		size_t Count = ( size_t ) impl::x_absolute_value<From, To>::value + 1>
	inline const std::array<int, Count> makeRange()
	{
		std::array<int, Count> tab = std::array<int, Count>();
		// ^ to silent runtime checks

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