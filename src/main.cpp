#include "rbtree.hpp"
#include "test.hpp"
#include <cstdlib>

int	main() {
	A				array[] = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
	int				size = sizeof(array) / sizeof(array[0]);
	ft::rbtree<A>	rbtree;

	std::cout << std::boolalpha;

	for (int i = 0; i < size; i++)
		rbtree[array[i]];
	
	rbtree.print();
}
