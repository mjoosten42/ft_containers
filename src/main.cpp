#include "tree.hpp"
#include "test.hpp"
#include <algorithm>
#include <array>
#include <map>

int	main() {
	A			array[] = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
	int			size = sizeof(array) / sizeof(array[0]);
	ft::tree<A>	tree;

	std::cout << std::boolalpha;

	for (int i = 0; i < size; i++)
		tree[array[i]];

	tree.print();
}
