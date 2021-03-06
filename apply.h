/*
* explode.h
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

/* INFO:
*  kmu::apply enables you to call every non-const callable object
*  with a std::tuple of parameters
*/

#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include "integer_sequence.h"

namespace kmu
{
	namespace impl
	{
		// VC12 workaround
		template <typename Functor, typename Tuple, size_t... Index>
		auto apply_helper(Functor&& functor, Tuple&& tupleOfArgs, 
							   integer_sequence<size_t, Index...>)
			-> decltype(std::forward<Functor>(functor)( 
				std::get<Index>(std::forward<Tuple>(tupleOfArgs))...))
		{
			return std::forward<Functor>(functor)(std::get<Index>( 
				std::forward<Tuple>(tupleOfArgs))...);
		}
	} // namespace impl

	template <typename Functor, typename Tuple>
	auto apply(Functor&& functor, Tuple&& tupleOfArgs)
		-> decltype(impl::apply_helper(std::forward<Functor>(functor), 
			std::forward<Tuple>(tupleOfArgs),
	make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>()))
	{
		return impl::apply_helper(std::forward<Functor>(functor), 
			std::forward<Tuple>(tupleOfArgs),
			make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>());
	}
} // namespace kmu