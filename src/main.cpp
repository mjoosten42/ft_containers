#include <iostream> // std::cout
#include <iomanip> // std::setw
#include <array>  // std::array

#if STD // CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	//#include <map.hpp>
	//#include <stack.hpp>
	#include <vector.hpp>
#endif

template <typename T>
std::ostream&	operator<<(std::ostream& os, const ft::vector<T>& v) {
	std::cout << "size: " << std::setw(2) << v.size();
	std::cout << ", cap: " << std::setw(2) << v.capacity();
	std::cout << " { ";
	for (size_t i = 0; i < v.size(); i++) {
		if (i)
			std::cout << ", ";
		std::cout << v[i];
	}
	std::cout << " }";
	return os;
}

template <typename T>
void	print(const T& value) {
	std::cout << value << std::endl;	
}

int	main() {

	std::array<int, 3>	array = { 0, 1, 2 };

	std::cout << std::boolalpha;

	{
		ft::vector<int>	v;

		v.insert(v.begin(), array.begin(), array.end());
		print(v);
		v.insert(v.begin(), 3, 42);
		print(v);
		v.insert(v.begin(), -1);
		print(v);

		std::fill(v.begin(), v.end(), 3);

	}
}
