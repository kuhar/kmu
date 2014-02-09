/*
* handyTest.cpp
*
*  Created on: Feb 06, 2014
*      Author: Jakub 'kuhar' Kuderski
*/


#include "stdafx.h"
#include "CppUnitTest.h"
#include "template_math.h"

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{

	TEST_CLASS( TemplateMathTest )
	{
	public:

		TEST_METHOD( DistanceTest )
		{
			Assert::AreEqual( 5, ( int ) kmu::distance<5, 0>::value );
			Assert::AreEqual( 5, ( int ) kmu::distance<0, 5>::value );
			Assert::AreEqual( 5, ( int ) kmu::distance<-5, 0>::value );
			Assert::AreEqual( 5, ( int ) kmu::distance<0, -5>::value );
			Assert::AreEqual( 0, ( int ) kmu::distance<0, 0>::value );
			Assert::AreEqual( 2, ( int ) kmu::distance<1, -1>::value );
			Assert::AreEqual( 2, ( int ) kmu::distance<-1, 1>::value );
		}

		TEST_METHOD( AbsoluteValueTest )
		{
			Assert::AreEqual( 5, ( int ) kmu::absolute_value<5>::value );
			Assert::AreEqual( 5, ( int ) kmu::absolute_value<-5>::value );

			Assert::AreEqual( 0, ( int ) kmu::absolute_value<0>::value );
		}

		TEST_METHOD( AdditionTest )
		{
			Assert::AreEqual( 12, ( int ) kmu::add_values<5, 7>::value );
			Assert::AreEqual( -2, ( int ) kmu::add_values<-5, 3>::value );

			Assert::AreEqual( 0, ( int ) kmu::add_values<0, 0>::value );
		}

		TEST_METHOD( SubtrationTest )
		{
			Assert::AreEqual( -2, ( int ) kmu::subtract_values<5, 7>::value );
			Assert::AreEqual( 2, ( int ) kmu::subtract_values<7, 5>::value );
			Assert::AreEqual( -8, ( int ) kmu::subtract_values<-5, 3>::value );

			Assert::AreEqual( 0, ( int ) kmu::subtract_values<0, 0>::value );
		}

		TEST_METHOD( MultiplicationTest )
		{
			Assert::AreEqual( 35, ( int ) kmu::multiply_values<7, 5>::value );
			Assert::AreEqual( -15, ( int ) kmu::multiply_values<-5, 3>::value );

			Assert::AreEqual( 0, ( int ) kmu::multiply_values<0, 0>::value );
		}

		TEST_METHOD( DivisionTest )
		{
			Assert::AreEqual( 1, ( int ) kmu::divide_values<7, 5>::value );
			Assert::AreEqual( -1, ( int ) kmu::divide_values<-5, 3>::value );
			Assert::AreEqual( 2, ( int ) kmu::divide_values<4, 2>::value );

			Assert::AreEqual( 0, ( int ) kmu::divide_values<0, 1>::value );
		}

		TEST_METHOD( IsNegativeTest )
		{
			Assert::AreEqual( false, ( bool ) kmu::is_negative<7>::value );
			Assert::AreEqual( true, ( bool ) kmu::is_negative<-7>::value );
			Assert::AreEqual( false, ( bool ) kmu::is_negative<0>::value );
		}

		TEST_METHOD( IsPositiveTest )
		{
			Assert::AreEqual( true, ( bool ) kmu::is_positive<7>::value );
			Assert::AreEqual( false, ( bool ) kmu::is_positive<-7>::value );
			Assert::AreEqual( false, ( bool ) kmu::is_positive<0>::value );
		}

		TEST_METHOD( RoundDivisionTest )
		{
			Assert::AreEqual( 1, ( int ) kmu::divide_and_round_values<7, 5>::value );
			Assert::AreEqual( 2, ( int ) kmu::divide_and_round_values<7, 4>::value );
			Assert::AreEqual( -2, ( int ) kmu::divide_and_round_values<7, -4>::value );
			Assert::AreEqual( 2, ( int ) kmu::divide_and_round_values<4, 2>::value );

			Assert::AreEqual( 0, ( int ) kmu::divide_and_round_values<0, 1>::value );
		}

		TEST_METHOD( CompareTest )
		{
			Assert::IsTrue( kmu::compare_values<7, 5>::is_grater );
			Assert::IsTrue( kmu::compare_values<5, 5>::is_greater_equal );
			Assert::IsFalse( kmu::compare_values<4, 5>::is_grater );

			Assert::IsFalse( kmu::compare_values<4, 5>::is_equal );
			Assert::IsTrue( kmu::compare_values<5, 5>::is_equal );
			Assert::IsFalse( kmu::compare_values<5, 5>::is_not_equal );

			Assert::IsTrue( kmu::compare_values<3, 5>::is_lesser );
			Assert::IsFalse( kmu::compare_values<7, 5>::is_lesser );
			Assert::IsFalse( kmu::compare_values<7, 5>::is_lesser_equal );
			Assert::IsTrue( kmu::compare_values<5, 5>::is_lesser_equal );
		}

	};
}