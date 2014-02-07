/*
* method_traits.h
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

#pragma once

#include <type_traits>
#include <tuple>
#include <functional>

#include "function_traits.h"

namespace kmu
{

	template<typename Signature>
	struct is_const_method;

	template<typename ReturnType, typename Clazz, typename... Args>
	struct is_const_method<ReturnType( Clazz::* ) ( Args... )>
		: public std::false_type
	{
	};

	template<typename ReturnType, typename Clazz, typename... Args>
	struct is_const_method<ReturnType( Clazz::* ) ( Args... ) const>
		: public std::true_type
	{
	};

	template<typename Signature>
	struct method_traits;

	template<typename ReturnType, typename Clazz, typename... Args>
	struct method_traits<ReturnType( Clazz::* ) ( Args... )>
	{
		using return_type = ReturnType;
		using class_name = Clazz;
		using is_const = std::false_type;
		using is_functor = std::false_type;

		enum : size_t
		{
			arity = sizeof... ( Args )
		};

		template<size_t N>
		struct argument
		{
			static_assert ( N < arity,
							"Given index is greater than count of arguments" );
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};

	};

	template<typename ReturnType, typename Clazz, typename... Args>
	struct method_traits<ReturnType( Clazz::* ) ( Args... ) const>
		: public method_traits<ReturnType( Clazz::* ) ( Args... )>
	{
		using is_const = std::true_type;
	};

	template<typename Functor>
	struct method_traits
	{
	private:
		using x_traits = method_traits<decltype( &Functor::operator() )>;

	public:
		using return_type = typename x_traits::return_type;
		using class_name = typename x_traits::class_name;
		using is_const = typename x_traits::is_const;
		using is_functor = std::true_type;

		enum : size_t
		{
			arity = x_traits::arity
		};

		template<size_t N>
		struct argument
		{
			using type = typename x_traits::template argument<N>::type;
		};
	};

} // namespace kmu