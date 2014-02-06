/*
* explode.h
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

/* INFO:
*  kmu::explode anables you to call every non-const callable object,
*  not taking parametrs by rvalue references, with a std::tuple of parameters
*/

#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include "handy.h"

namespace kmu
{

	template <typename Index, typename... Args>
	struct FunctorCaller;

	template <typename ReturnType, size_t... Index>
	struct FunctorCaller<ReturnType, impl::x_index_tuple_type<Index...>>
	{
		template <typename FunctorType, typename TupleType>
		static inline ReturnType call( FunctorType&& functor, TupleType&& tupleArgs )
		{
			return functor( std::forward<decltype( std::get<Index>( 
				std::forward<TupleType>( tupleArgs ) ))>
							( std::get<Index>( std::forward<TupleType>( tupleArgs ) ) )... );
		}

		template <typename FunctorType, typename TupleType>
		static inline ReturnType call( FunctorType&& functor, const TupleType& tupleArgs )
		{
			return functor( std::get<Index>( tupleArgs )... );
		}
	};

	template <int... Index>
	struct FunctorCaller<void, impl::x_index_tuple_type<Index...>>
	{
		template <typename FunctorType, typename TupleType>
		static inline void call( FunctorType&& functor, TupleType&& tupleArgs )
		{
			functor( std::forward<decltype( std::get<Index>(
				std::forward<TupleType>( tupleArgs ) ) )>
				( std::get<Index>( std::forward<TupleType>( tupleArgs ) ) )... );
		}

		template <typename FunctorType, typename TupleType>
		static inline void call( FunctorType&& functor, const TupleType& tupleArgs )
		{
			functor( std::get<Index>( tupleArgs )... );
		}
	};

	template <typename Functor, typename... Args,
		typename ReturnType = impl::x_result_of<Functor( Args... )>::type>
	inline typename std::enable_if<std::is_same<ReturnType, void>::value, void>::type
		explode( Functor&& functor, const std::tuple<Args...>& tupleArgs )
	{
			FunctorCaller<ReturnType, impl::x_index_tuple<sizeof...( Args )>>
				::call( std::forward<Functor>( functor ), tupleArgs );
	}

	template <typename Functor, typename... Args,
		typename ReturnType = impl::x_result_of<Functor( Args... )>::type>
	inline typename std::enable_if<!std::is_same<ReturnType,
		void>::value, ReturnType>::type
		explode( Functor&& functor, const std::tuple<Args...>& tupleArgs )
	{
		return FunctorCaller<ReturnType, impl::x_index_tuple<sizeof...( Args )>>
			::call( std::forward<Functor>( functor ), tupleArgs );
	}

	template <typename Functor, typename... Args, 
		typename ReturnType = impl::x_result_of<Functor( Args... )>::type>
	inline typename std::enable_if<std::is_same<ReturnType, void>::value, void>::type
		explode( Functor&& functor, std::tuple<Args...>& tupleArgs )
	{
		FunctorCaller<ReturnType, impl::x_index_tuple<sizeof...( Args )>>
			::call( std::forward<Functor>( functor ), tupleArgs );
	}

	template <typename Functor, typename... Args,
		typename ReturnType = impl::x_result_of<Functor( Args... )>::type>
	inline typename std::enable_if<!std::is_same<ReturnType,
		void>::value, ReturnType>::type
		explode( Functor&& functor, std::tuple<Args...>& tupleArgs )
	{
		return FunctorCaller<ReturnType, impl::x_index_tuple<sizeof...( Args )>>
			::call( std::forward<Functor>( functor ), tupleArgs );
	}

	template <typename Functor, typename... Args,
		typename ReturnType = impl::x_result_of<Functor( Args... )>::type>
	inline typename std::enable_if<std::is_same<ReturnType, void>::value, void>::type
		explode( Functor&& functor, std::tuple<Args...>&& tupleArgs )
	{
		FunctorCaller<ReturnType, impl::x_index_tuple<sizeof...( Args )>>
			::call( std::forward<Functor>( functor ),
			std::forward<decltype( tupleArgs )>( tupleArgs ) );
	}

	template <typename Functor, typename... Args,
		typename ReturnType = impl::x_result_of<Functor( Args... )>::type>
	inline typename std::enable_if<!std::is_same<ReturnType,
		void>::value, ReturnType>::type
		explode( Functor&& functor, std::tuple<Args...>&& tupleArgs )
	{
		return FunctorCaller<ReturnType, impl::x_index_tuple<sizeof...( Args )>>
			::call( std::forward<Functor>( functor ),
			std::forward<decltype( tupleArgs )>( tupleArgs ) );
	}

} // namespace kmu