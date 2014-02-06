/*
* handyTest.cpp
*
*  Created on: Feb 06, 2014
*      Author: Jakub 'kuhar' Kuderski
*/


#include "stdafx.h"
#include "CppUnitTest.h"
#include "handy.h"

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{

	TEST_CLASS( HandyTest )
	{
	public:

		TEST_METHOD( AscendingRangeTest )
		{
			auto range = kmu::makeRange<0, 10>();
			Assert::AreEqual( range.size(), 11u );
			for( size_t i = 0; i < range.size(); ++i )
			{
				Assert::AreEqual( ( int ) i, range[i] );
			}

			range = kmu::makeRange<10, 20>();
			Assert::AreEqual( range.size(), 11u );
			for( size_t i = 0; i < range.size(); ++i )
			{
				Assert::AreEqual( ( int ) i + 10, range[i] );
			}

			auto emptyRange = kmu::makeRange<0,0>();
			Assert::AreEqual( 1u, emptyRange.size() );
		}

		TEST_METHOD( DescendingRangeTest )
		{
			auto range = kmu::makeRange<10, 0>();
			Assert::AreEqual( range.size(), 11u );
			for( size_t i = 0; i < range.size(); ++i )
			{
				Assert::AreEqual( ( int ) ( range.size() - i - 1 ), range[i] );
			}

			auto secondRange = kmu::makeRange<10, -10>();
			Assert::AreEqual( secondRange.size(), 21u );
			for( int value = 10, i = 0; value >= -10; --value, ++i )
			{
				Assert::AreEqual( value, secondRange[i] );
			}
		}

		TEST_METHOD( MakeVectorOfElementsTest )
		{
			auto value = kmu::makeVectorOfElements( std::string( "meow" ) );
			Assert::AreEqual( 1u, value.size() );
			Assert::AreEqual( std::string( "meow" ), value.front() );

			value = kmu::makeVectorOfElements( std::string( "purr" ), 16 );
			Assert::AreEqual( 16u, value.size() );
			Assert::AreEqual( std::string( "purr" ), value.back() );
		}

	};
}