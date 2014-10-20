/*
* type_list.h
*
*  Created on: Oct 19, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

#pragma once

#include "handy.h"

namespace kmu
{
	
	template<typename... Ts>
	struct type_list
	{
		static const size_t size = sizeof... (Ts);
		static const bool is_empty = size > 0;
		static const bool are_all_unique = are_all_unique<Ts...>::value;

		template<size_t index>
		struct at : get_type_at<index, Ts...> {};

		template<typename... Args>
		struct append : identity<type_list<Ts..., Args...>> {};

		template<typename... Args>
		struct prepend : identity<type_list<Args..., Ts...>> {};
	};

	template<typename T>
	struct is_type_list : std::false_type {};

	template<typename... Ts>
	struct is_type_list<type_list<Ts...>> : std::true_type {};

	template<size_t index, typename T>
	struct element_at : T::template at<index> {};

	template<size_t index, typename T>
	using element_at_t = typename element_at<index, T>::type;

	template<typename T, typename... Ts>
	struct append : T::template append<Ts...> {};

	template<typename T, typename... Ts>
	using append_t = typename append<T, Ts...>::type;

	template<typename T, typename... Ts>
	struct prepend : T::template prepend<Ts...> {};

	template<typename T, typename... Ts>
	using prepend_t = typename prepend<T, Ts...>::type;

	template<typename...>
	struct type_list_cat;
	
	template<>
	struct type_list_cat<> : identity<type_list<>> {};

	template<typename... Ts>
	struct type_list_cat<type_list<Ts...>> : identity<type_list<Ts...>> {};

	template<typename... Ts, typename... Xs, typename... Rest>
	struct type_list_cat<type_list<Ts...>, type_list<Xs...>, Rest...>
		: type_list_cat<type_list<Ts..., Xs...>, Rest...> {};

	template<typename... Ts>
	using type_list_cat_t = typename type_list_cat<Ts...>::type;

	template<typename>
	struct head;

	template<typename First, typename... Rest>
	struct head<type_list<First, Rest...>> : identity<First> {};

	template<typename T>
	using head_t = typename head<T>::type;

	template<typename>
	struct tail;

	template<typename First, typename... Rest>
	struct tail<type_list<First, Rest...>> : identity<type_list<Rest...>> {};

	template<typename T>
	using tail_t = typename tail<T>::type;

	namespace impl
	{
		template<typename Accumulator, size_t begin, size_t end, size_t current, typename...>
		struct slice_helper;

		template<typename Accumulator, size_t begin, size_t end, typename... Rest>
		struct slice_helper<Accumulator, begin, end, end, Rest...> : identity<Accumulator> {};

		template<typename Accumulator, size_t begin, size_t end, size_t current,
				typename First, typename... Rest>
		struct slice_helper<Accumulator, begin, end, current, First, Rest...>
			: slice_helper<
				if_t<(current >= begin && current < end), 
					append_t<Accumulator, First>,
					Accumulator
				>, begin, end, current + 1, Rest...>
		{};
	}

	template<typename, size_t begin, size_t end>
	struct slice;

	template<size_t begin, size_t end, typename... Ts>
	struct slice<type_list<Ts...>, begin, end> 
		: impl::slice_helper<type_list<>, begin, end, 0, Ts...>
	{
		static_assert( begin <= end, "Begin exceeds end" );
		static_assert( end <= sizeof... ( Ts ), "End exceeds the size of type_list" );
	};

	template<typename T, size_t begin, size_t end>
	using slice_t = typename slice<T, begin, end>::type;

	template<typename T, size_t end>
	struct slice_to : slice<T, 0, end> {};

	template<typename T, size_t end>
	using slice_to_t = typename slice_to<T, end>::type;

	template<typename T, size_t from>
	struct slice_from : slice<T, from, T::size> {};

	template<typename T, size_t from>
	using slice_from_t = typename slice_from<T, from>::type;

} // namespace kmu