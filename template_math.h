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
		static const bool value = Value < 0 ? true : false;
	};

	template<int Value>
	struct is_positive
	{
		static const bool value = Value > 0 ? true : false;
	};

	template<int FirstValue, int SecondValue>
	struct add_values
	{
		static const int value = FirstValue + SecondValue;
	};

	template<int FirstValue, int SecondValue>
	struct subtract_values
	{
		static const int value = FirstValue - SecondValue;
	};

	template<int FirstValue, int SecondValue>
	struct multiply_values
	{
		static const int value = FirstValue * SecondValue;
	};

	template<int FirstValue, int SecondValue>
	struct divide_values
	{
		static_assert( SecondValue != 0, "Division by 0" );
		static const int value = FirstValue / SecondValue;
	};

	template<int FirstValue, int SecondValue>
	struct modulo_values
	{
		static_assert(SecondValue != 0, "Division by 0");
		static const int value = FirstValue % SecondValue;
	};

	template<int FirstValue, int SecondValue>
	struct distance
	{

		static const int value = (FirstValue - SecondValue) < 0
			? -(FirstValue - SecondValue) : (FirstValue - SecondValue);
	};

	template<int Value>
	struct absolute_value : distance<Value, 0>
	{
	};

	template<int FirstValue, int SecondValue>
	struct divide_and_round_values
	{
		static_assert(SecondValue != 0, "Division by 0");
	private:
		static const int x_potential_additional = is_negative<multiply_values<FirstValue, 
										SecondValue>::value>::value ? -1 : 1;
			
		static const int x_additional =  (((FirstValue % SecondValue) * 2) 
						>= absolute_value<SecondValue>::value) ? x_potential_additional : 0;

	public:
		static const int value =  ((2 * FirstValue / SecondValue) + x_additional) / 2;
	};

	template<int FirstValue, int SecondValue>
	struct compare_values
	{
		static const bool is_grater = FirstValue > SecondValue;
		static const bool is_greater_equal = FirstValue >= SecondValue;
		static const bool is_equal = FirstValue == SecondValue;
		static const bool is_not_equal = !is_equal;
		static const bool is_lesser = FirstValue < SecondValue;
		static const bool is_lesser_equal = FirstValue <= SecondValue;
	};

	template<bool FirstValue, bool SecondValue>
	struct logical_operator
	{
		static const bool logical_or = FirstValue || SecondValue;
		static const bool logical_nor = !FirstValue || !SecondValue;
		static const bool logical_xor = FirstValue != SecondValue;
		static const bool logical_xnor = !logical_xor;

		static const bool logical_and = FirstValue && SecondValue;
		static const bool logical_nand = !FirstValue && !SecondValue;

		static const bool logical_eql = !logical_xor;
	};

	template<int FirstValue, int SecondValue>
	struct bitwise_operator
	{
		static const int bitwise_or = FirstValue | SecondValue;
		static const int bitwise_xor = FirstValue ^ SecondValue;

		static const int bitwise_and = FirstValue & SecondValue;

		static const int bitwise_eql = ~bitwise_xor;

		static const int bitwise_shift_left = FirstValue << SecondValue;
		static const int bitwise_shift_right = FirstValue >> SecondValue;
	};

} // namespace kmu