#include <array>
#include <set>
#include <sstream>

// #define PRINT
#include "test.hpp"

#ifdef STD 
	#include <vector>
	namespace ft = std;
#else
	#include "vector.hpp"
#endif

template <typename T, typename U>
std::ostream&	operator<<(std::ostream& os, const ft::vector<T, U>& v) {
	std::cout << "size: " << std::setw(2) << v.size();
	std::cout << " { ";
	for (size_t i = 0; i < v.size(); i++) {
		if (i)
			std::cout << ", ";
		std::cout << v[i];
	}
	std::cout << " }";
	return os;
}

int	main() {
	std::cout << std::boolalpha;

	clock_t	t = clock();
	const A	a(42);

	{

	const std::array<A, 3>	array = { 1, 2, 3 };
	std::set<A> set;

	set.insert(4);
	set.insert(5);
	set.insert(6);

	ft::vector<A>	v(array.begin(), array.end());
	ft::vector<A>	u(set.begin(), set.end());

	{
		print("Constructors\n");
		std::istringstream	iss("7 8 9");
	
		ft::vector<A>	v1; print(v1);
		ft::vector<A>	v3(3, 42); print(v3);
		ft::vector<A>	v4a(array.begin(), array.end()); print(v4a);
		ft::vector<A>	v4b(set.begin(), set.end()); print(v4b);
		ft::vector<A>	v4c((std::istream_iterator<A>(iss)), std::istream_iterator<A>()); print(v4c);
		ft::vector<A>	v5(v); print(v5);

		print("Copy assignment\n");
		v5 = v3;
		print(v5);
	}

	{
		print("assign\n");
		v.assign(5, 42);
		print(v);
		v.assign(array.begin(), array.end());
		print(v);
		v.assign(set.begin(), set.end());
		print(v);
	
		std::istringstream	iss("7 8 9");
		v.assign(std::istream_iterator<A>(iss), std::istream_iterator<A>());
		print(v);
	}

	{
		print("Element access\n");

		try { print(v.at(0)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v.at(-1)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v.at(3)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v.at(SIZE_T_MAX)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		print(v[0]);
		print(v[1]);
		print(v[2]);
		print(v.front());
		print(v.back());
		print(*v.data());
	}

	{
		print("Iterators\n");
		print(v);
		for (ft::vector<A>::iterator it = v.begin(); it != v.end(); it++) {
			*it = (*it + *it);
			print(*it);			
		}
	
		for (ft::vector<A>::const_iterator it = v.begin(); it != v.end(); it++) {
			print(*it);			
		}
	
		for (ft::vector<A>::reverse_iterator it = v.rbegin(); it != v.rend(); it++) {
			*it = *it / 2;
			print(*it);			
		}
	
		for (ft::vector<A>::const_reverse_iterator it = v.rbegin(); it != v.rend(); it++) {
			print(*it);			
		}
	}

	{
		print("Capacity\n");
		print(v);
		print(v.empty());
		v.clear();
		print(v);
		print(v.empty());
			
		print(v.max_size());
		print(v.get_allocator().max_size());

		v.reserve(10);
		print(v.capacity() >= 10);
		print(v.size());
	}

	print("Modifiers\n");

	{
		print("insert\n");
		std::istringstream	iss("7 8 9");
	
		print(*v.insert(v.begin(), a));
		print(*v.insert(v.end() - 1, a));
		v.insert(v.begin(), 3, 42);
		v.insert(v.begin() + 1, 2, a);
		v.insert(v.begin(), array.begin(), array.end());
		v.insert(v.begin() + 0, set.begin(), set.end());
		v.insert(v.begin() + 3, array.begin(), array.end());
		v.insert(v.begin() + 6, std::istream_iterator<A>(iss), std::istream_iterator<A>());
		print(v);
		print("erase\n");
		print(*v.erase(v.begin()));
		print(*v.erase(v.begin() + 2));
		print(*v.erase(v.begin(), v.begin() + 1));
		print(*v.erase(v.begin() + 1, v.begin() + 3));
		print(*v.erase(v.begin() + 3, v.end() - 1));
		print(v);
		print(*v.erase(v.begin(), v.end()));
		print(v);
	}

	{
		print("push/pop\n");
		for (int i = 0; i < 20; i++)
			v.push_back(a);
		print(v);
		for (int i = 0; i < 10; i++)
			v.pop_back();
		print(v);
		
		print("resize\n");
		v.resize(3, a);
		print(v);
		v.resize(10, 2);
		print(v);
	}

	{
		print("comparisons\n");
		v.assign(array.begin(), array.end());
		u.assign(set.begin(), set.end());
		print(v);
		print(u);
		print(v == u);
		print(v != u);
		print(v >  u);
		print(v >= u);
		print(v <  u);
		print(v <= u);
		v.assign(set.begin(), set.end());
		print(v == u);
		print(v != u);
		print(v >  u);
		print(v >= u);
		print(v <  u);
		print(v <= u);
	}

	{
		print("swap\n");
	
		v.clear();
		u.clear();
		for (int i = 0; i < 50; i++) {
			v.push_back(i);
			v.swap(u);
		}
		print(v);
		print(u);
	}

	}

	if (a.alive() != 1)
		std::cout << "Still alive: " << a.alive() - 1 << "\n";

	#ifdef STD
		std::cout << "\nstd: ";
	#else
		std::cout << "\nft: ";
	#endif

	t = clock() - t;
	std::cout << 1000 * ((float)t / CLOCKS_PER_SEC) << " ms\n";
}
