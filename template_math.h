/*
* template_math.h
*
*  Created on: Feb 09, 2014
*      Author: Jakub "kuhar" Kuderski
*/

#pragma once

#include <cstddef>
#include <type_traits>

namespace kmu
{

	template<int Value>
	struct is_negative
	{
		enum : bool
		{
			value = Value < 0 ? true : false
		};
	};

	template<int Value>
	struct is_positive
	{
		enum : bool
		{
			value = Value > 0 ? true : false
		};
	};

	template<int FirstValue, int SecondValue>
	struct add_values
	{
		enum : int
		{
			value = FirstValue + SecondValue
		};
	};

	template<int FirstValue, int SecondValue>
	struct subtract_values
	{
		enum : int
		{
			value = FirstValue - SecondValue
		};
	};

	template<int FirstValue, int SecondValue>
	struct multiply_values
	{
		enum : int
		{
			value = FirstValue * SecondValue
		};
	};

	template<int FirstValue, int SecondValue>
	struct divide_values
	{
		static_assert( SecondValue != 0, "Division by 0" );
		enum : int
		{
			value = FirstValue / SecondValue
		};
	};

	template<int FirstValue, int SecondValue>
	struct modulo_values
	{
		static_assert( SecondValue != 0, "Division by 0" );
		enum : int
		{
			value = FirstValue % SecondValue
		};
	};

	template<int FirstValue, int SecondValue>
	struct divide_and_round_values
	{
		static_assert( SecondValue != 0, "Division by 0" );
	private:

		enum : int
		{
			x_potential_additional = is_negative<multiply_values<FirstValue, 
										SecondValue>::value>::value ? -1 : 1,
			
			x_additional =  ( ( ( FirstValue % SecondValue ) * 2 ) 
						>= absolute_value<SecondValue>::value ) ? x_potential_additional : 0
		};

	public:
		
		enum : int
		{
			value =  ( ( 2 * FirstValue / SecondValue ) + x_additional ) / 2
		};
	};

	template<int FirstValue, int SecondValue>
	struct distance
	{
		enum : int
		{
			value = ( FirstValue - SecondValue ) < 0 ?
			-( FirstValue - SecondValue ) : ( FirstValue - SecondValue )
		};
	};

	template<int Value>
	struct absolute_value : distance<Value, 0>
	{
	};

	template<int FirstValue, int SecondValue>
	struct compare_values
	{
		enum : bool
		{
			is_grater = FirstValue > SecondValue,
			is_greater_equal = FirstValue >= SecondValue,
			is_equal = FirstValue == SecondValue,
			is_not_equal = !is_equal,
			is_lesser = FirstValue < SecondValue,
			is_lesser_equal = FirstValue <= SecondValue
		};
	};

	template<bool FirstValue, bool SecondValue>
	struct logical_operator
	{
		enum : bool
		{
			or = FirstValue || SecondValue,
			nor = !FirstValue || !SecondValue,
			xor = FirstValue ^ SecondValue,
			xnor = !xor,

			and = FirstValue && SecondValue,
			nand = !FirstValue && !SecondValue,

			eql = !xor
		};
	};

	template<int FirstValue, int SecondValue>
	struct bitwise_operator
	{
		enum : int
		{
			or = FirstValue | SecondValue,
			xor = FirstValue ^ SecondValue,

			and = FirstValue & SecondValue,

			eql = ~xor,

			shift_left = FirstValue << SecondValue,
			shift_right = FirstValue >> SecondValue
		};
	};


} // namespace kmu