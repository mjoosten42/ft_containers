#include "redBlackTree.hpp"
#include "test.hpp"
#include <map>
#include <algorithm>

int	main() {
	ft::redBlackTree<int>			rbt;
	ft::redBlackTree<int>::iterator it;

	std::cout << std::boolalpha;

	rbt.insert(8);
	rbt.insert(3);
	rbt.insert(1);
	rbt.insert(6);
	rbt.insert(4);
	rbt.insert(7);
	rbt.insert(10);
	rbt.insert(14);
	rbt.insert(13);

	rbt.print(rbt.root());
	print("--------------------------");

	while (rbt.size()) {
		rbt.erase(rbt.begin());
		rbt.print(rbt.root());
		print("--------------------------");
	}

}
