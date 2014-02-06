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
			auto range = kmu::makeRange<10>();
			Assert::AreEqual( range.size(), 10u );
			for( size_t i = 0; i < range.size(); ++i )
			{
				Assert::AreEqual( range[i], i );
			}

			auto emptyRange = kmu::makeRange<0>();
		}

		TEST_METHOD( DescendingRangeTest )
		{
			auto range = kmu::makeDescendingRange<10>();
			Assert::AreEqual( range.size(), 10u );
			for( size_t i = 0; i < range.size(); ++i )
			{
				Assert::AreEqual( range[i], range.size() - i - 1 );
			}

			auto emptyRange = kmu::makeDescendingRange<0>();
		}

		TEST_METHOD( MakeVectorOfElementsTest )
		{
			auto value = kmu::makeVectorOfElements( std::string( "meow" ) );
			Assert::AreEqual( value.size(), 1u );
			Assert::AreEqual( value.front(), std::string( "meow" ) );

			value = kmu::makeVectorOfElements( std::string( "purr" ), 16 );
			Assert::AreEqual( value.size(), 16u );
			Assert::AreEqual( value.back(), std::string( "purr" ) );
		}

	};
}