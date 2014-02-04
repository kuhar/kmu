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
	struct method_traits;

	template<typename ReturnType, typename Clazz, typename... Args>
	struct method_traits<ReturnType( Clazz::* ) ( Args... )>
	{
		using return_type = ReturnType;
		using class_name = Clazz;
		using is_const = std::false_type;

		template<int N>
		struct argument
		{
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};

		enum
		{
			argument_count = sizeof... ( Args )
		};
	};

	template<typename ReturnType, typename Clazz, typename... Args>
	struct method_traits<ReturnType( Clazz::* ) ( Args... ) const> :
		public method_traits<ReturnType( Clazz::* ) ( Args... )>
	{
		using is_const = std::true_type;
	};


} // namespace kmu