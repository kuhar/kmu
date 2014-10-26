/*
* algorithms.h
*
*  Created on: Oct 19, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

#pragma once

#include "handy.h"
#include "type_list.h"

namespace kmu
{
	namespace impl
	{
		template<template <typename...> class Func, typename ArgsPack>
		struct apply_helper;

		template<template <typename...> class Func, typename... Ts>
		struct apply_helper<Func, type_list<Ts...>> : Func<Ts...> {};

		template<template <typename...> class Func, typename ArgsPack>
		struct apply_not_helper;

		template<template <typename...> class Func, typename... Ts>
		struct apply_not_helper<Func, type_list<Ts...>> 
			: bool_constant<Func<Ts...>::value == false> {};
	}

	template<template <typename...> class Func, typename ArgsPack>
	using apply = impl::apply_helper<Func, ArgsPack>;

	template<template <typename...> class Func, typename ArgsPack>
	using apply_not = impl::apply_not_helper<Func, ArgsPack>;

	template<template <typename...> class Func, typename... Args>
	using requires_t = void_t<type_t<std::enable_if<Func<Args...>::value>>>;

	template<template <typename...> class Func, typename... Args>
	using requires_not_t = void_t<type_t<std::enable_if<!Func<Args...>::value>>>;

	template<bool value>
	using when_t = type_t<std::enable_if<value>>;

	template<template <typename> class Func, typename ArgsPack>
	struct all_of
		: bool_constant<
			Func<head_t<ArgsPack>>::value && all_of<Func, tail_t<ArgsPack>>::value> {};

	template<template <typename> class Func>
	struct all_of<Func, type_list<>> : bool_constant<true> {};

	template<template <typename> class Func, typename ArgsPack>
	struct none_of
		: bool_constant<
			!Func<head_t<ArgsPack>>::value && none_of<Func, tail_t<ArgsPack>>::value> {};

	template<template <typename> class Func>
	struct none_of<Func, type_list<>> : bool_constant<true> {};

	template<template <typename> class Func, typename ArgsPack>
	struct any_of
		: bool_constant<
			Func<head_t<ArgsPack>>::value || any_of<Func, tail_t<ArgsPack>>::value> {};

	template<template <typename> class Func>
	struct any_of<Func, type_list<>> : bool_constant<false> {};

	template<template <typename> class Func, typename ArgsPack>
	struct count_if
		: size_t_constant<size_t(Func<head_t<ArgsPack>>::value)
			+ count_if<Func, tail_t<ArgsPack>>::value> {};

	template<template <typename> class Func>
	struct count_if<Func, type_list<>> : size_t_constant<0> {};

	template<template <typename> class Func, typename ArgsPack>
	struct count_if_not
		: size_t_constant<(Func<head_t<ArgsPack>>::value ? 0 : 1)
			+ count_if_not<Func, tail_t<ArgsPack>>::value> {};

	template<template <typename> class Func>
	struct count_if_not<Func, type_list<>> : size_t_constant<0> {};
	
} // namespace kmu