#include "redBlackTree.hpp"
#include "test.hpp"
#include <map>

int	main() {
	ft::redBlackTree<int>	rbt;
	// ft::pair<ft::redBlackTree<int>::iterator, bool>	pair;

	std::cout << std::boolalpha;

	rbt.insert(8);
	// rbt.insert(3);
	// rbt.insert(1);
	// rbt.insert(6);
	// rbt.insert(4);
	// rbt.insert(7);
	// rbt.insert(10);
	// rbt.insert(14);
	// rbt.insert(13);

	// rbt.print(rbt.root());

	std::cout << "\n";

	for (ft::redBlackTree<int>::iterator it = rbt.begin(); it != rbt.end(); ++it)
		print(*it);
}
