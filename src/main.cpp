#include "rbtree.hpp"
#include "test.hpp"

#include <cstdlib>

void	randomTree(ft::rbtree<A>& rbtree, int insert = 32, int erase = 64) {
	srand(time(NULL));
	for (int i = 0; i < insert; i++)
		rbtree.insert(10 + rand() % 90);
	for (int i = 0; i < erase; i++)
		rbtree.erase(10 + rand() % 90);
}

void	wikiTree(ft::rbtree<A>& rbtree) {
	A		array[] = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
	int		size = sizeof(array) / sizeof(array[0]);

	for (int i = 0; i < size; i++)
		rbtree.insert(array[i]);
}

int	main() {
	ft::rbtree<A>	rbtree;

	std::cout << std::boolalpha;

	randomTree(rbtree);
	rbtree.print();
}
