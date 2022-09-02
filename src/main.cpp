#include <string>
#include <iostream>
#include "../tests/myInt.hpp"

#if 0
	#include <map>
	namespace ft = std;
#else
	#include "map.hpp"
#endif

typedef ft::map<myInt, std::string>	Map;

int	main() {
	const ft::pair<myInt, std::string>	array[] = {
		{ 8,  "h" },
		{ 3,  "c" },
		{ 1,  "a" },
		{ 6,  "f" },
		{ 4,  "d" },
		{ 7,  "g" },
		{ 10, "j" },
		{ 14, "n" },
		{ 13, "m" }
	};
	const uint	size = sizeof(array) / sizeof(array[0]);

	Map	map(array, array + size);
	Map n(array + 2, array + 5);

	n = map;

	//std::cout << (n == map) << "\n";
}
