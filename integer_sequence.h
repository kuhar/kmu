/*
* integer_sequence.h
*
*  Created on: Feb 09, 2014
*      Author: Jakub "kuhar" Kuderski
*/

#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace kmu
{
	template<typename Type, Type... Index>
	struct integer_sequence
	{
		template<Type Number>
		using prepend = integer_sequence<Type, Number, Index...>;
	};

	namespace impl
	{
		template<typename Type, typename Number, typename ReturnType>
		struct x_make_integer_sequence_helper;

		template<typename Type, typename ReturnType>
		struct x_make_integer_sequence_helper<Type, std::integral_constant<Type, 0>, ReturnType>
		{
			using type = ReturnType;
		};

		template<typename Type, typename Number, typename ReturnType>
		struct x_make_integer_sequence_helper
			: x_make_integer_sequence_helper<Type, std::integral_constant<Type, Number::value - 1>,
			typename ReturnType::template prepend<Number::value - 1>>
		{
		};
	} // namespace impl

	template<typename Type, Type Number>
	using make_integer_sequence = typename impl::x_make_integer_sequence_helper<Type,
		std::integral_constant<Type, Number>, integer_sequence<Type >>::type;

	template <size_t... Index>
	using index_sequence = integer_sequence<size_t, Index...>;

	template <size_t Number>
	using make_index_sequence = make_integer_sequence<size_t, Number>;

	template <typename... Args>
	using index_sequence_for = make_index_sequence<sizeof... ( Args )>;

	namespace impl
	{
		static_assert( std::is_same<make_integer_sequence<int, 0>,
					   integer_sequence<int>>::value, "integer_sequence error" );
		static_assert( std::is_same<make_integer_sequence<int, 1>,
					   integer_sequence<int, 0>>::value, "integer_sequence error" );
		static_assert( std::is_same<make_integer_sequence<int, 2>,
					   integer_sequence<int, 0, 1>>::value, "integer_sequence error" );
		static_assert( std::is_same<make_integer_sequence<int, 3>,
					   integer_sequence<int, 0, 1, 2>>::value, "integer_sequence error" );
	} // namespace impl
} // namespace kmu