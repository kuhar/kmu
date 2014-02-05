/*
* explodeTest.cpp
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "method_traits.h"
#include <functional>
#include <tuple>
#include <string>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{

	TEST_CLASS( MethodTraitsTest )
	{
	public:

		TEST_METHOD( BasicMethodTraitsTest )
		{
			Assert::IsTrue( std::is_same<kmu::method_traits<
							decltype( &testStruct::isEmpty )>::return_type, void >::value );
			Assert::AreEqual( ( int ) kmu::method_traits<
							decltype( &testStruct::isEmpty )>::arity, 0 );
			Assert::IsTrue( 
				kmu::method_traits<decltype( &testStruct::isEmpty )>::is_const::value );
			
		}

		TEST_METHOD( TwoArgMethodTraitsTest )
		{
			Assert::IsTrue( std::is_same<kmu::method_traits<
							decltype( &testStruct::twoArg )>::return_type, int& >::value );
			Assert::AreEqual( ( int ) kmu::method_traits<
							  decltype( &testStruct::twoArg )>::arity, 2 );
			Assert::IsFalse(
				kmu::method_traits<decltype( &testStruct::twoArg )>::is_const::value );

			Assert::IsTrue( std::is_same<kmu::method_traits<decltype(
				&testStruct::twoArg )>::argument<0>::type, float>::value );
			
			Assert::IsTrue( std::is_same<kmu::method_traits<decltype(
				&testStruct::twoArg )>::argument<1>::type, bool>::value );

			Assert::IsTrue( is_same<kmu::method_traits<decltype(
				&testStruct::twoArg )>::return_type, int&>::value );
		}

		TEST_METHOD( FunctorMethodTraitsTest )
		{
			std::function<size_t( int, char, bool&)> firstLambda 
				= []( int a, char b, bool& c ) -> size_t { return 0; };
			
			Assert::IsTrue( kmu::method_traits<decltype( firstLambda )>::is_functor::value );
			Assert::IsTrue( kmu::method_traits<decltype( firstLambda )>::is_const::value );

			Assert::IsTrue( std::is_same<kmu::method_traits<decltype( firstLambda )>
							::return_type, size_t>::value );

			Assert::AreEqual( (int) kmu::method_traits<decltype( firstLambda )>::arity, 3 );
			Assert::IsTrue( std::is_same<kmu::method_traits<decltype( firstLambda )>
							::argument<0>::type, int>::value );
			Assert::IsTrue( std::is_same<kmu::method_traits<decltype( firstLambda )>
							::argument<1>::type, char>::value );
			Assert::IsTrue( std::is_same<kmu::method_traits<decltype( firstLambda )>
							::argument<2>::type, bool&>::value );
		}

	private:

		struct testStruct
		{
			void isEmpty() const;

			int& twoArg( float, bool );
		};

	};
}