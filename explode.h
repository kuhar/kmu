/*
* explode.h
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

#pragma once

#include <tuple>
#include <type_traits>

namespace kmu
{
	namespace impl // Implementation
	{
		template <int... Index>
		struct x_index_tuple_type
		{
			template <int N>
			using append = x_index_tuple_type<Index..., N>;
		};

		template <int N>
		struct x_make_index
		{
			using type = typename x_make_index<N - 1>::type::template append<N - 1>;
		};

		template <>
		struct x_make_index<0>
		{
			using type = x_index_tuple_type<>;
		};

		template <int N>
		using x_index_tuple = typename x_make_index<N>::type;

	} // namespace impl

	template <typename Index, typename... Args>
	struct FunctorCaller;

	template <typename ReturnType, int... Index>
	struct FunctorCaller<ReturnType, impl::x_index_tuple_type<Index...>>
	{
		template <typename FunctorType, typename TupleType>
		static inline ReturnType call( FunctorType&& functor, TupleType&& tupleArgs )
		{
			return functor( std::get<Index>( std::forward<TupleType>( tupleArgs ) )... );
		}
	};

	template <typename Functor, typename... Args>
	inline auto explode( Functor&& functor, const std::tuple<Args...>& tupleArgs )
		-> typename std::result_of<Functor( Args&&... )>::type
	{
		return FunctorCaller < std::result_of<Functor( Args&&... )>::type,
			impl::x_index_tuple < sizeof...( Args ) >>
			::call( std::forward<Functor>( functor ),
			std::forward<decltype( tupleArgs )>( tupleArgs ) );
	}
	template <typename Functor, typename... Args>
	inline auto explode( Functor&& functor, std::tuple<Args...>& tupleArgs )
		-> typename std::result_of<Functor( Args&&... )>::type
	{
		return FunctorCaller < std::result_of<Functor( Args&&... )>::type,
			impl::x_index_tuple < sizeof...( Args ) >>
			::call( std::forward<Functor>( functor ),
			std::forward<decltype( tupleArgs )>( tupleArgs ) );
	}

	template <typename Functor, typename... Args>
	inline auto explode( Functor&& functor, std::tuple<Args...>&& tupleArgs )
		-> typename std::result_of<Functor( Args&&... )>::type
	{
		return FunctorCaller < std::result_of<Functor( Args&&... )>::type,
			impl::x_index_tuple < sizeof...( Args ) >>
			::call( std::forward<Functor>( functor ),
			std::forward<decltype( tupleArgs )>( tupleArgs ) );
	}

} // namespace kmu