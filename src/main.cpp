#if 1
	#include <vector>
	#include <stack>
	#include <map>
	#include <set>
	namespace ft = std;
#else
	#include "vector.hpp"
	#include "stack.hpp"
	#include "map.hpp"
	#include "set.hpp"
#endif

#include <iostream>
#include <ctime>
#include <string>
#include "../tests/myInt.hpp"
#include "../tests/myAllocator.hpp"

typedef ft::vector<myInt>			Vec;
typedef ft::map<myInt, std::string>	Map;

#define COUNT 1 << 16

int	main() {
	std::cout << std::boolalpha;
	std::clock_t	start = clock();

	Vec	v;
	for (uint i = 0; i < COUNT; i++)
		v.resize(i);
	std::cout << "time: " << (float)(clock() - start) / (float)CLOCKS_PER_SEC << "s\n";
}
