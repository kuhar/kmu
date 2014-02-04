/*
* explodeTest.cpp
*
*  Created on: Feb 04, 2014
*      Author: Jakub 'kuhar' Kuderski
*/


#include "stdafx.h"
#include "CppUnitTest.h"
#include "explode.h"
#include <functional>
#include <tuple>
#include <string>
#include <utility>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace Tests
{		
	string dummyFunction( int a, string&& b )
	{
		b += to_string( a );
		return b;
	}

	struct TemplateFunctor
	{
		template <typename A, typename B>
		auto operator() ( A a, B b ) -> decltype( a * b ) const
		{
			return a * b;
		}
	};

	int fn( int )
	{
		return int();
	}
	
	TEST_CLASS( ExplodeTest )
	{
	public:
		
		TEST_METHOD( BasicExplodeTest )
		{
			Assert::AreEqual( kmu::explode( &dummyMethod, make_tuple( 2, 2.5f ) ), 5.0f );

			Assert::AreEqual( kmu::explode( &dummyFunction, make_tuple( 0, "Ala" ) ),
							  string( "Ala0" ) );

			Assert::AreEqual( kmu::explode( []() { return true;  }, make_tuple() ), true );
		}

		TEST_METHOD( FunctorExplodeTest )
		{
			struct Whatever
			{
				int operator() ( float a )
				{
					return ( int ) a;
				}

				bool operator() ( char a )
				{
					return false;
				}
			};
			tuple<float> whateverTuple;
			get<0>( whateverTuple ) = 3.2f;

			Assert::AreEqual( kmu::explode( Whatever(), whateverTuple ), 3 );
			Assert::AreEqual( kmu::explode( Whatever(), make_tuple( 'a' ) ), false );

			TemplateFunctor functor;
			const auto constTuple = make_tuple( 5, 2.5f );
			Assert::AreEqual( kmu::explode( std::move(functor), constTuple ), 12.5f );

		}

		TEST_METHOD( LambdaExplodeTest )
		{
			auto&& lambda = []( int a, bool b, char& c ) { return a != 0 && b && c != 'a'; };
			char someChar = 'x';
			Assert::IsFalse( kmu::explode( lambda, forward_as_tuple( 5, false, someChar ) ) );

			Assert::AreEqual( kmu::explode( 
				[]( const char* pCString ) { return string( pCString ); },
				forward_as_tuple( "foobar" ) ), string( "foobar" ) );
		}


	private:

		static float dummyMethod( int a, float b )
		{
			return ( float ) a * b;
		}

	};
}