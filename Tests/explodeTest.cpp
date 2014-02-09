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
			Assert::AreEqual( 5.0f, kmu::explode( &dummyMethod, make_tuple( 2, 2.5f ) ) );

			Assert::AreEqual( string( "Ala0" ), 
							  kmu::explode( &dummyFunction, make_tuple( 0, "Ala" ) ) );

			Assert::AreEqual( true, kmu::explode( []() { return true;  }, make_tuple() ) );
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

			Assert::AreEqual( 3, kmu::explode( Whatever(), whateverTuple ) );
			Assert::AreEqual( false, kmu::explode( Whatever(), make_tuple( 'a' ) ) );

			TemplateFunctor functor;
			const auto constTuple = make_tuple( 5, 2.5f );
			Assert::AreEqual( 12.5f, kmu::explode( std::move( functor ), constTuple ) );
		}

		TEST_METHOD( LambdaExplodeTest )
		{
			auto&& lambda = []( int& a, bool b, char& c ) { return a != 0 && b && c != 'a'; };
			char someChar = 'x';
			int x = 5;
			Assert::IsFalse( kmu::explode( lambda, forward_as_tuple( x, false, someChar ) ) );

			Assert::AreEqual( string( "foobar" ), kmu::explode(
				[]( const char* pCString ) { return string( pCString ); },
				forward_as_tuple( "foobar" ) ) );
		}

	private:

		static float dummyMethod( int a, float b )
		{
			return ( float ) a * b;
		}

	};
}