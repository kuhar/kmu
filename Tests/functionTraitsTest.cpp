/*
* functionTraitsTest.cpp
*
*  Created on: Feb 06, 2014
*      Author: Jakub 'kuhar' Kuderski
*/

#include "stdafx.h"
#include "CppUnitTest.h"
#include "function_traits.h"
#include <functional>
#include <tuple>
#include <string>
#include <type_traits>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{

	void firstFunction();
	bool& secondFunction( void*, char );

	TEST_CLASS( FunctionTraitsTest )
	{
	public:

		TEST_METHOD( BasicFunctionTraitsTest )
		{
			Assert::IsTrue( std::is_same<kmu::function_traits<
							decltype( firstFunction )>::return_type, void >::value );
			Assert::AreEqual( 0, ( int ) kmu::function_traits<
							  decltype( firstFunction )>::arity );

			Assert::IsTrue( std::is_same<kmu::function_traits<
							decltype( secondFunction )>::return_type, bool& >::value );
			Assert::AreEqual( 2, ( int ) kmu::function_traits<
							  decltype( secondFunction )>::arity );
			Assert::IsTrue( std::is_same< kmu::function_traits<
							decltype( secondFunction )>::argument<0>::type, void* >::value );
			Assert::IsTrue( std::is_same<kmu::function_traits<
							decltype( secondFunction )>::argument<1>::type, char >::value );
		}

		TEST_METHOD( MethodFunctionTraitsTest )
		{
			Assert::IsTrue( std::is_same<kmu::function_traits<
							decltype( &testStruct::isEmpty )>::return_type, void >::value );
			Assert::AreEqual( 1, ( int ) kmu::function_traits<
							  decltype( &testStruct::isEmpty )>::arity );

			Assert::IsTrue( std::is_same<kmu::function_traits<
							decltype( &testStruct::twoArg )>::return_type, int& >::value );
			Assert::AreEqual( 3, ( int ) kmu::function_traits<
							  decltype( &testStruct::twoArg )>::arity );
			Assert::IsTrue( std::is_same< kmu::function_traits<
							decltype( &testStruct::twoArg )>::argument<0>::type,
							testStruct >::value );
			Assert::IsTrue( std::is_same<kmu::function_traits<
							decltype( &testStruct::twoArg )>::argument<1>::type,
							float >::value );
			Assert::IsTrue( std::is_same<kmu::function_traits<
							decltype( &testStruct::twoArg )>::argument<2>::type,
							bool >::value );
		}

		TEST_METHOD( FunctorFunctionTraitsTest )
		{
			std::function<size_t( int, char, bool& )> firstLambda;

			Assert::IsTrue( std::is_same<kmu::function_traits<decltype( firstLambda )>
							::return_type, size_t>::value );

			Assert::AreEqual( 4, ( int ) kmu::function_traits<decltype( firstLambda )>::arity );
			Assert::IsTrue( std::is_same<kmu::function_traits<decltype( firstLambda )>
							::argument<1>::type, int>::value );
			Assert::IsTrue( std::is_same<kmu::function_traits<decltype( firstLambda )>
							::argument<2>::type, char>::value );
			Assert::IsTrue( std::is_same<kmu::function_traits<decltype( firstLambda )>
							::argument<3>::type, bool&>::value );
		}

	private:

		struct testStruct
		{
			void isEmpty() const;

			int& twoArg( float, bool );
		};

	};
}