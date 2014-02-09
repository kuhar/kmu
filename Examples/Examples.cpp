#include <iostream>
#include <tuple>
#include <utility>
#include <functional>
#include <vector>

#include "explode.h"

using namespace std;
using namespace kmu;

int main()
{
	explode( []( int ){ cout << "Wazzza?"; }, make_tuple( 1 ) );

	return 0;
}