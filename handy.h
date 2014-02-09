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

		template<typename Signature> // GCC's implementation
		class x_result_of;

		template<typename Functor, typename... ArgTypes>
		struct x_result_of<Functor( ArgTypes... )>
		{
			typedef decltype( std::declval<Functor>()( std::declval<ArgTypes>()... ) ) type;
		};

		template<int FirstValue, int SecondValue = 0>
		struct x_distance
		{
			enum
			{
				value = ( FirstValue - SecondValue ) < 0 ? 
				-( FirstValue - SecondValue ) : ( FirstValue - SecondValue )
			};
		};

	} // namespace impl

	template<typename T>
	std::vector<T> makeVectorOfElements( const T& element, size_t count = 1 )
	{
		return std::vector<T>( count, element );
	}

	template<int From, int To,
		size_t Count = ( size_t ) impl::x_distance<From, To>::value + 1>
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