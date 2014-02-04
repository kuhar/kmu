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
							decltype( &testStruct::isEmpty )>::argument_count, 0 );
		}


	private:

		struct testStruct
		{
			void isEmpty() const;
		};

	};
}