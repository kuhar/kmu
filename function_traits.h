/*
* function_traits.h
*
*  Created on: Feb 05, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

#pragma once

#include <type_traits>
#include <tuple>

namespace kmu
{

	template<typename Signature>
	struct function_traits;

	template<typename ReturnType, typename... Args>
	struct function_traits<ReturnType( Args... )>
	{
		using return_type = ReturnType;

		enum : size_t
		{
			arity = sizeof... ( Args )
		};

		template <std::size_t N>
		struct argument
		{
			static_assert ( N < arity,
							"Given index is greater than count of arguments" );
			using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
		};
	};

	template<typename ReturnType, typename... Args>
	struct function_traits<ReturnType(*)( Args... )>
		: public function_traits<ReturnType( Args... )>
	{
	};

	template<typename ReturnType, typename Clazz, typename... Args>
	struct function_traits<ReturnType( Clazz::* )( Args... )>
		: public function_traits<ReturnType( Clazz, Args... )>
	{
	};

	template<typename ReturnType, typename Clazz, typename... Args>
	struct function_traits<ReturnType( Clazz::* )( Args... ) const>
		: public function_traits<ReturnType( Clazz, Args... )>
	{
	};

	template<typename Functor>
	struct function_traits
	{
	private:
		using x_traits = function_traits<decltype( &Functor::operator() )>;

	public:
		using return_type = typename x_traits::return_type;

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