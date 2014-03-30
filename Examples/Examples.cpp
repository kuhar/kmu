#include <iostream>
#include <string>
#include <vector>

#include "explode.h"
#include "variant.h"

using namespace std;
using namespace kmu;

struct someClass
{
	~someClass()
	{
		cout << "\nAnihilation\n";
	}
};

int main()
{
	//explode( []( int ){ cout << "Wazzza?"; }, make_tuple( 1 ) );
	Variant<int, char, vector<int>> myVariant;
	myVariant.set<int>( 5 );
	cout << myVariant.get<int>();
	myVariant.set<char>( 'A' );
	cout << myVariant.get<char>();
	myVariant.set<char>( 'Z' );
	cout << myVariant.get<char>() << myVariant.getCurrentTypeID().name();

	decltype( myVariant ) otherVariant = myVariant;
	Variant<int, const char*, someClass> stringIntVariant;
	stringIntVariant.set<const char*>( "\ntest\n" );
	cout << stringIntVariant.get<const char*>() << sizeof( stringIntVariant );
	stringIntVariant.set<someClass>();
	stringIntVariant.set<someClass>();

	return 0;
}