#include "vector"

#include <sstream>
#include <iostream>

int	main() {
	std::istringstream	iss("7 8 9");

	std::vector<int>	d((std::istream_iterator<int>(iss)), std::istream_iterator<int>());

	std::cout << d.capacity() << "\n";
}
