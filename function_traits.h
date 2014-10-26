/*
* function_traits.h
*
*  Created on: Feb 05, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

#pragma once

#include <type_traits>
#include "type_list.h"

namespace kmu
{

	template<typename>
	struct function_traits;

	template<typename ReturnType, typename... Args>
	struct function_traits<ReturnType(Args...)>
	{
		using return_type = ReturnType;

		static const size_t	arity = sizeof... (Args);

		template <std::size_t N>
		struct argument : get_type_at<N, Args...>
		{
			static_assert(N < arity,
							"Given index is greater than the number of arguments");
		};
	};

	template<typename ReturnType, typename... Args>
	struct function_traits<ReturnType(*)(Args...)>
		: function_traits<ReturnType(Args...)> {};

	template<typename ReturnType, typename Class, typename... Args>
	struct function_traits<ReturnType(Class::*)(Args...)>
		: function_traits<ReturnType(Class*, Args...)> {};

	template<typename ReturnType, typename Class, typename... Args>
	struct function_traits<ReturnType(Class::*)(Args...) const>
		: function_traits<ReturnType(const Class*, Args...)> {};

	template<typename ReturnType, typename Class, typename... Args>
	struct function_traits<ReturnType(Class::*)(Args...) volatile>
		: function_traits<ReturnType(volatile Class*, Args...)> {};

	template<typename ReturnType, typename Class, typename... Args>
	struct function_traits<ReturnType(Class::*)(Args...) const volatile>
		: function_traits<ReturnType(const volatile Class*, Args...)> {};

	template<typename Functor>
	struct function_traits
	{
	private:
		using x_traits = function_traits<decltype(&Functor::operator())>;

	public:
		using return_type = typename x_traits::return_type;

		static const size_t arity = x_traits::arity

		template<size_t N>
		struct argument 
			: identity<typename x_traits::template argument<N>::type> {};
	};
	
} // namespace kmu