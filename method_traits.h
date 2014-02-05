/*
* method_traits.h
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

#pragma once

#include <type_traits>
#include <tuple>

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

		enum : size_t
		{
			argument_count = sizeof... ( Args )
		};

		template<size_t N>
		struct argument
		{
			static_assert ( N < argument_count,
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


} // namespace kmu