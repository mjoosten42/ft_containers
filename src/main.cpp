#if 0
	#include <map>
	namespace ft = std;
#else
	#include "map.hpp"
#endif

#include <string>
#include "../tests/myInt.hpp"
#include "../tests/myAllocator.hpp"

typedef ft::map<myInt, std::string>	Map;

int	main() {
	std::cout << std::boolalpha;
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

	Map	m(array, array + size);

}
