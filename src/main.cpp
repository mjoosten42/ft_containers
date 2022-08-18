#include "rbtree.hpp"
#include "test.hpp"

#include <cstdlib>

void	randomTree(ft::rbtree<A>& rbtree, int insert, int erase) {
	srand(time(NULL));
	for (int i = 0; i < insert; i++)
		rbtree.insert(10 + rand() % 90);
	rbtree.print();
	for (int i = 0; i < erase; i++)
		rbtree.erase(10 + rand() % 90);
}

void	wikiTree(ft::rbtree<A>& rbtree) {
	A		array[] = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
	int		size = sizeof(array) / sizeof(array[0]);

	for (int i = 0; i < size; i++)
		rbtree[array[i]];
}

int	main() {
	ft::rbtree<A>	rbtree;

	std::cout << std::boolalpha;

	randomTree(rbtree, 32, 64);
	rbtree.print();

	// ft::rbtree<A>::iterator it = rbtree.begin();
	// ft::rbtree<A>::iterator ite = rbtree.end();

	// it++;
	// rbtree.erase(it, ite);
	// rbtree.print();
}
