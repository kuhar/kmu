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
#include "algorithms.h"

namespace kmu
{
	namespace impl
	{
		template<typename Functor, typename... Ts>
		class curry_helper
		{
		public:
			template<typename Tuple>
			curry_helper(Functor&& fn, Tuple&& args)
				: m_functor(std::forward<Functor>(fn))
				, m_args(std::forward<Tuple>(args))
			{}

			decltype(auto) destructiveCall()
			{
				return kmu::apply(std::move(m_functor), std::move(m_args));;
			}

			decltype(auto) operator()() &&
			{
				return destructiveCall();
			}

			decltype(auto) operator()() &
			{
				return m_Caller.call(*this);
			}

			decltype(auto) operator()() const&
			{
				return kmu::apply(m_functor, m_args);
			}

			template<typename... Rs>
			curry_helper<Functor, Ts..., Rs&&...> operator()(Rs&&... rs) &&
			{
				return {std::move(m_functor), std::tuple_cat(std::move(m_args),
					std::forward_as_tuple(std::forward<Rs>(rs)...))};
			}

			template<typename... Rs>
			curry_helper<Functor, Ts..., Rs&&...> operator()(Rs&&... rs) const&
			{
				return {m_functor, std::tuple_cat(m_args,
					std::forward_as_tuple(std::forward<Rs>(rs)...))};
			}

		private:
			using ArgsType = std::tuple<Ts...>;

			template<typename Dummy, bool UseCopyCtor = true>
			struct Caller
			{
				decltype(auto) call(curry_helper& that)
				{
					return kmu::apply(that.m_functor, that.m_args);
				}
			};

			template<typename Dummy>
			struct Caller<Dummy, false>
			{
				decltype(auto) call(curry_helper& that)
				{
					assert(wasCalled == false && "Functor already called");
					wasCalled = true;
					return that.destructiveCall();
				}
				bool wasCalled = false;
			};

			ArgsType m_args;
			Functor m_functor;
			Caller<void, kmu::all_of<std::is_copy_constructible, 
				kmu::type_list<kmu::remove_ref_t<Ts>...>>::value> m_Caller;
		};
	}

	template<typename Functor>
	impl::curry_helper<Functor> curry(Functor&& f)
	{
		return{std::forward<Functor>(f), std::make_tuple()};
	}

} // namespace kmu