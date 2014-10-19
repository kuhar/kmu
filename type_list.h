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

} // namespace kmu