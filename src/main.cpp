#include <string>
#include <iostream>
#include "../tests/myInt.hpp"

#if 0
	#include <map>
	namespace ft = std;
#else
	#include "map.hpp"
#endif

int	main() {
	ft::pair<myInt, std::string>	array[] = {
		{ 8,  "h" },
		{ 3,  "c" },
		{ 1,  "a" }
	};
	uint	size = sizeof(array) / sizeof(array[0]);

	ft::map<myInt, std::string>	map(array, array + size);

	std::cout << map.size() << "\n";

}
