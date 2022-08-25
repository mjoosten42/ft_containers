#include "meta.hpp"
#include "vector.hpp"

#include <sstream>
#include <iostream>

int	main() {
	ft::vector<int>	v;
	ft::vector<int>	w;
	
	v.resize(10);
	w = v;

	std::cout << (v == w) << "\n";
}
