#include "rbtree.hpp"
#include "myInt.hpp"

#include <cstdlib>

void	randomTree(ft::rbtree<myInt>& rbtree, int insert = 32, int erase = 64) {
	srand(time(NULL));
	for (int i = 0; i < insert; i++)
		rbtree.insert(10 + rand() % 90);
	for (int i = 0; i < erase; i++)
		rbtree.erase(10 + rand() % 90);
}

void	wikiTree(ft::rbtree<myInt>& rbtree) {
	myInt	array[] = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
	int		size = sizeof(array) / sizeof(array[0]);

	for (int i = 0; i < size; i++)
		rbtree.insert(array[i]);
}

int	main() {
	ft::rbtree<myInt>	rbtree;

	std::cout << std::boolalpha;

	randomTree(rbtree);
	rbtree.print();
}
