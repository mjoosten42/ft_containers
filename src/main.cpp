#include <iostream> // std::cout
#include <iomanip> // std::setw
#include <array>  // std::array
#include "meta.hpp"

#ifdef STD
	#include <vector>
	using std::vector;
#else
	#include "vector.hpp"
	using ft::vector;
#endif

#define PRINT(x) std::cout << std::setw(4) << __LINE__ << "| " << x << std::endl;

template <typename T>
void	printVector(const vector<T>& v, size_t line) {
	std::cout << std::setw(4) << line << "| vector: ";
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << ' ';
	std::cout << std::endl;
}

int	main() {

	enable_if<is_pointer<int>::value, int> a; (void)a;


	std::cout << std::boolalpha;

	//	Member types
	{
		vector<int>	v;
	
		vector<int>::value_type				a; (void)a;
		vector<int>::allocator_type			b; (void)b;
		vector<int>::size_type				c; (void)c;
		vector<int>::difference_type		d; (void)d;
		vector<int>::reference				e = *v.begin(); (void)e;
		vector<int>::const_reference		f = *v.begin(); (void)f;
		vector<int>::pointer				g; (void)g;
		vector<int>::const_pointer			h; (void)h;
		vector<int>::iterator				i; (void)i;
		vector<int>::const_iterator			j; (void)j;
		vector<int>::reverse_iterator		k; (void)k;
		vector<int>::const_reverse_iterator	l; (void)l;
	}

	std::cout << "    |\n    | Constructors\n";

	{
		std::array<int, 3>	array = { 0, 1, 2 };
	
		vector<int>	v1(3);
		vector<int>	v2(3, 1);
		vector<int>	v3(array.begin(), array.end());
		vector<int>	v4(v1);
		printVector(v1, __LINE__);
		printVector(v2, __LINE__); 
		printVector(v3, __LINE__);
		printVector(v4, __LINE__);
		v4 = v3;
		printVector(v4, __LINE__);
		v4 = vector<int>();
		printVector(v4, __LINE__);
	}

	std::cout << "    |\n    | Element access\n";

	{
		vector<int>	v;

		int *i = &v.front(); PRINT(i);
		i = &v.back(); PRINT(i);
		// PRINT(v[0]); segfault
		try { PRINT(v.at(0)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		PRINT(v.front());
		PRINT(v.back());
		try { PRINT(v.at(0)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { PRINT(v.at(4)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { PRINT(v[0]); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }	
		try { PRINT(v[4]); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }	
	}

	std::cout << "    |\n    | Iterators\n";

	{
		vector<int>	v;

		vector<int>::iterator			begin;
		vector<int>::iterator			end;
		vector<int>::reverse_iterator	rbegin;
		vector<int>::reverse_iterator	rend;

		begin = v.begin();
		end = v.end();
		rbegin = v.rbegin();
		rend = v.rend();
		PRINT(*(void **)&begin);
		PRINT(*(void **)&end);
		PRINT(*(void **)&rbegin);
		PRINT(*(void **)&rend);
		for (int i = 0; i < 10; i++)
			v.push_back(i);
		begin = v.begin();
		end = v.end();
		rbegin = v.rbegin();
		rend = v.rend();
		PRINT(*(void **)&begin);
		PRINT(*(void **)&end);
		PRINT(*(void **)&rbegin);
		PRINT(*(void **)&rend);
		PRINT(*begin);
		PRINT(*--end);
		PRINT(*rbegin);
		PRINT(*--rend);

		// const vector<int>::iterator			cbegin;
		// const vector<int>::iterator			cend;
		// const vector<int>::reverse_iterator	crbegin;
		// const vector<int>::reverse_iterator	crend;
	
		// PRINT(*(void **)&cbegin);
		// PRINT(*(void **)&cend);
		// PRINT(*(void **)&crbegin);
		// PRINT(*(void **)&crend);
	}

	std::cout << "    |\n    | Capacity\n";

	{
		vector<int>	v;

		PRINT(v.empty());
		PRINT(v.size());
		PRINT(v.capacity());
		v.push_back(42);
		PRINT(v.empty());
		PRINT(v.size());
		PRINT(v.capacity());
		v.pop_back();
		PRINT(v.empty());
		PRINT(v.capacity());
		PRINT(v.max_size());
	}

	std::cout << "    |\n    | Modifiers\n";

	{
		vector<int>	v;

		v.clear();
		v.push_back(42);
		v.clear();
		PRINT(v.size());
		v.push_back(0);
		v.push_back(1);
		v.push_back(2);
		PRINT(v.capacity());
		v.insert(v.begin() + 1, 1);
		PRINT(v.capacity());
		PRINT(v[0]);
		PRINT(v[1]);
		PRINT(v[2]);
		PRINT(v[3]);
	}
}
