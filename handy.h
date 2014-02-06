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

		template<size_t... Index>
		inline const std::array<size_t, sizeof... ( Index )>
			x_make_ascending_range( x_index_tuple_type<Index...> )
		{
			return { Index... };
		}

		template<size_t... Index, size_t Count = sizeof... ( Index )>
		inline const std::array<size_t, sizeof... ( Index )>
			x_make_descending_range( x_index_tuple_type<Index...> )
		{
			return { ( Count - Index - 1 )... };
		}

	} // namespace impl

	template<typename T>
	std::vector<T> makeVectorOfElements( const T& element, size_t count = 1 )
	{
		assert( count > 0 );
		return std::vector<T>( count, element );
	}

	template<size_t Count>
	inline const std::array<size_t, Count> makeRange()
	{
		return impl::x_make_ascending_range( impl::x_index_tuple<Count>() );
	}

	template<size_t Count>
	inline const std::array<size_t, Count> makeDescendingRange()
	{
		return impl::x_make_descending_range( impl::x_index_tuple<Count>() );
	}

} // namespace kmu