/*
* handy.h
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

/* INFO:
*  Bunch of uncategorized utils
*/

#pragma once

#include <type_traits>
#include <typeindex>
#include <vector>
#include <array>
#include <tuple>
#include <cassert>

#include "template_math.h"

namespace kmu
{
	struct Constants
	{
		static const size_t Size_T_Max = -1;
		static const unsigned char UChar_Max = -1;
	};
	
	namespace impl // Implementation
	{
		template<typename Signature> // GCC's implementation
		struct x_result_of;

		template<typename Functor, typename... ArgTypes>
		struct x_result_of<Functor( ArgTypes... )>
		{
			typedef decltype( std::declval<Functor>()( std::declval<ArgTypes>()... ) ) type;
		};

		template<typename T>
		using x_result_of_t = typename x_result_of<T>::type;

		template<size_t Index, typename Type, typename... Ts>
		struct get_index_of_type_helper;

		template<size_t Index, typename Type, typename First, typename... Rest>
		struct get_index_of_type_helper<Index, Type, First, Rest...>
		{
			static const size_t value = std::is_same<Type, First>::value
							? Index : get_index_of_type_helper<Index + 1, Type, Rest...>::value;
		};

		template<size_t Index, typename Type>
		struct get_index_of_type_helper< Index, Type>
		{
			static const size_t value = Constants::Size_T_Max;
		};

	} // namespace impl

	template<typename T>
	struct identity
	{
		using type = T;
	};
	
	template<typename T>
	using identity_t = typename identity<T>::type;

	namespace impl
	{
		template<typename...>
		struct voider : identity<void> {};
	}

	template<typename... Ts>
	using void_t = typename impl::voider<Ts...>::type;

	template<bool value, typename First, typename Second>
	using if_t = typename std::conditional<value, First, Second>::type;

	template<bool value>
	using bool_constant = if_t<value, std::true_type, std::false_type>;

	template<typename First, typename... Rest>
	struct is_one_of;

	template<typename First, typename... Rest>
	struct is_one_of<First, First, Rest...> : std::true_type {};

	template<typename First, typename Second, typename... Rest>
	struct is_one_of<First, Second, Rest...> : is_one_of<First, Rest...> {};

	template<typename Only>
	struct is_one_of<Only> : std::false_type {};

	template<typename...>
	struct are_all_unique;

	template<typename First, typename... Rest>
	struct are_all_unique<First, Rest...> 
		: bool_constant<!kmu::is_one_of<First, Rest...>::value
						&& kmu::are_all_unique<Rest...>::value> {};

	template<typename Only>
	struct are_all_unique<Only> : std::true_type {};

	template<size_t Index, typename... Ts>
	struct get_type_at
	{
		static_assert( sizeof... ( Ts ) > 0, "The list of types cannot be empty" );
	private:
		using tuple_type = std::tuple<Ts...>;
	public:
		using type = typename std::tuple_element<Index, tuple_type>::type;
	};

	template<size_t Index, typename... Ts>
	using get_type_at_t = typename get_type_at<Index, Ts...>::type;

	template<typename... Ts>
	inline std::type_index getTypeIndexOfTypeAt( size_t index )
	{
		assert( index < sizeof... ( Ts ) && "Given index exceeds the number of types" );
		static const std::type_index types[] = { typeid( Ts )... };
		return types[index];
	}

	template<typename SearchedType, typename... Types>
	struct get_index_of_type
	{
		static const size_t value = impl::get_index_of_type_helper<0, SearchedType, Types...>::value;
		static_assert( value != Constants::Size_T_Max, "Type not found" );
	};

	template<typename T>
	std::vector<T> makeVectorOfElements( const T& element, size_t count = 1 )
	{
		return std::vector<T>( count, element );
	}

	template<int From, int To,
		size_t Count = (size_t) kmu::distance<From, To>::value + 1>
	inline const std::array<int, Count> makeRange()
	{
		std::array<int, Count> tab;

		int step = (To - From) < 0 ? -1 : 1;
		int startValue = From;
		for( int i = 0; i < Count; ++i )
		{
			tab[i] = startValue;
			startValue += step;
		}

		return tab;
	}
} // namespace kmu