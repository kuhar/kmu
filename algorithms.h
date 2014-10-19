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

	template<template <typename...> class Func, typename... Args>
	using apply = Func<Args...>;

	template<template <typename...> class Predicate, typename... Args>
	using apply_not = bool_constant<Predicate<Args...>::value>;

	template<template <typename...> class Func, typename... Args>
	using requires_t = typename std::enable_if<Func<Args...>::value>::type;

	template<template <typename...> class Func, typename... Args>
	using requires_not_t = typename std::enable_if<!Func<Args...>::value>::type;

	template<template <typename> class, typename...>
	struct all_of;

	template<template <typename> class Predicate, typename First, typename... Rest>
	struct all_of<Predicate, First, Rest...>
		: bool_constant<
			Predicate<First>::value && all_of<Predicate, Rest...>::value > {};

	template<template <typename> class Predicate>
	struct all_of<Predicate> : bool_constant<true> {};

	template<template <typename> class, typename...>
	struct none_of;

	template<template <typename> class Predicate, typename First, typename... Rest>
	struct none_of<Predicate, First, Rest...>
		: bool_constant<
			!Predicate<First>::value && none_of<Predicate, Rest...>::value> {};

	template<template <typename> class Predicate>
	struct none_of<Predicate> : bool_constant<true> {};

	template<template <typename> class, typename...>
	struct any_of;

	template<template <typename> class Predicate, typename First, typename... Rest>
	struct any_of<Predicate, First, Rest...>
		: bool_constant<
			Predicate<First>::value || any_of<Predicate, Rest...>::value> {};

	template<template <typename> class Predicate>
	struct any_of<Predicate> : bool_constant<false> {};

	template<template <typename> class, typename...>
	struct count_if;

	template<template <typename> class Predicate, typename First, typename... Rest>
	struct count_if<Predicate, First, Rest...>
		: size_t_constant<size_t( Predicate<First>::value )
						+ count_if<Predicate, Rest...>::value> {};

	template<template <typename> class Predicate>
	struct count_if<Predicate> : size_t_constant<0> {};
	
} // namespace kmu