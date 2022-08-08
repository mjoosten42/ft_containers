#include "redBlackTree.hpp"
#include "test.hpp"
#include <algorithm>
#include <array>
#include <map>

int	main() {
	std::array<A, 9>	array = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
	int					size = sizeof(array) / sizeof(array[0]);
	ft::redBlackTree<A>	rbt;

	std::cout << std::boolalpha;

	for (int i = 0; i < size; i++)
		rbt.insert(array[i]);

	rbt.print();
	print("--------------------------");

	rbt.erase(8);
	rbt.print();
	print("--------------------------");
}
