/*
* curry.h
*
*  Created on: Nov 10, 2014
*      Author: Jakub 'kuhar' Kuderski
*
*/

#pragma once

#include <type_traits>
#include <memory>

#include "apply.h"

namespace kmu
{
	namespace impl
	{
		template<typename F, typename... Ts>
		struct curry_helper
		{
			template<typename Tuple>
			curry_helper(F&& f, Tuple&& a)
				: f(std::forward<F>(f))
				, args(std::forward<Tuple>(a))
			{}

			decltype(auto) operator()()
			{
				return kmu::apply(f, args);
			}

			decltype(auto) operator()() const
			{
				return kmu::apply(f, args);
			}

			template<typename... Rs>
			curry_helper<F, Ts..., Rs...> operator()(Rs&&... rs) const
			{
				return {f, std::tuple_cat(args,
					std::forward_as_tuple(std::forward<Rs>(rs)...))};
			}

			F f;
			std::tuple<Ts...> args;
		};
	}

	template<typename F>
	impl::curry_helper<F> curry(F&& f)
	{
		return {std::forward<F>(f), std::make_tuple()};
	}

} // namespace kmu