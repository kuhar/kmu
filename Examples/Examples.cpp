#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "explode.h"
#include "variant.h"
#include "type_list.h"

using namespace std;
using namespace kmu;

struct someClass
{
	someClass()
	{
	}
	
	~someClass()
	{
		cout << "Annihilation:\t" << id << endl;
	}

	someClass( someClass&& )
	{
		cout << "move ctor\n";
	}

	someClass( const someClass& )
	{
		cout << "copy ctor\n";
	}

	int id = 0;

	struct animus 
	{
		int wtw = 3;
	};
};

int main()
{
	explode( []( int ){ cout << "Wazzza?"; }, make_tuple( 1 ) );
	Variant<int, char, vector<int>> myVariant;
	myVariant.set<int>( 5 );
	cout << myVariant.get<int>();
	myVariant.set<char>( 'A' );
	cout << myVariant.get<char>();
	myVariant.set<char>( 'Z' );
	cout << myVariant.get<char>();
	myVariant.set<vector<int>>( 3, 5 );
	cout << myVariant.getCurrentTypeIndex().name();

	decltype( myVariant ) otherVariant = myVariant;
	Variant<int, const char*, someClass> stringIntVariant;
	stringIntVariant.set<const char*>( "\ntest\n" );
	cout << stringIntVariant.get<const char*>() << sizeof( stringIntVariant );
	stringIntVariant.set<someClass>();
	stringIntVariant.set<someClass>();
	stringIntVariant.reset();

	Variant<int, std::reference_wrapper<int>> test;
	test.set<int>( 5 );
	int a = 6;
	test.set<reference_wrapper<int>>( a );
	int& b = test.get<std::reference_wrapper<int>>();
	b = 7;
	cout << endl << a;
	cout << get<std::reference_wrapper<int>>( test ) << sizeof( std::ref(b) );

	Variant<int, int&, const int&> referenceTest;
	referenceTest.set<int&>( b );
	referenceTest.get<int&>() = 8;
	cout << get<int&>( referenceTest ) << endl;
	referenceTest.set<const int&>( a );
	cout << get<const int&>( referenceTest ) << endl;

	Variant<nullptr_t, someClass>original;
	original.set<someClass>();
	auto copyTest = original;
	copyTest.get<someClass>().id = 3;
	original = decltype( original )( copyTest );
	cout << original.get<1>().id << endl;
	::get<1>( original ).id = 12;
	cout << original.get<someClass>().id << endl;
	original.set<someClass>( someClass() );
	auto moveTest = std::move( original );
	cout << sizeof( moveTest ) << endl;
	cout << sizeof( moveTest.getCurrentTypeIndex() ) << endl;
	cout << sizeof( someClass ) << endl;

	cout << type_index( 
		typeid( kmu::get_type_at<3, int, char, bool, void, short>::type ) ).name() << "\n";
	cout << getTypeIndexOfTypeAt<int, char, bool, void, short>(3).name() << "\n";
	cout << kmu::get_index_of_type<void, bool, char, int, short, void, long>::value << "\n";
	cout << kmu::get_index_of_type<void, bool, char, int, short, void>::value << "\n";

	//debugTellType(kmu::get_type_at<3, int, char, bool, void, short>::type());
	for( auto&& x : makeRange<3, 13>() )
	{
		cout << x << "\t";
	}

	using first = type_list<void, int, char, long>;
	using second = prepend_t<type_list<void, int, char, long>, string, char*>;
	//debugTellType(type_list<void, int, char, long>::at<4>::type());
	//debugTellType(type_list<void, int, char, long>::append<string, char*>::type());
	//debugTellType(prepend_t<type_list<void, int, char, long>, string, char*>());
	//debugTellType(type_list_cat_t<first, first, first>());
	return 0;
}