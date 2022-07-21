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

template <typename T>
void	print(const T& value) {
	std::cout << value << std::endl;	
}

template <typename T>
void	printVector(const vector<T>& v) {
	std::cout << "size: " << v.size() << ", cap: " << v.capacity() << " { ";
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << ' ';
	std::cout << "}\n";
}

int	main() {

	std::array<int, 10>	array = { 0, 1, 1, 2, 3, 5, 8, 13, 23, 34 };

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

	std::cout << "\tConstructors\n";

	{
	
		vector<int>	v1(3);
		vector<int>	v2(3, 1);
		vector<int>	v3(array.begin(), array.end());
		vector<int>	v4(v1);
		vector<int>	v5;
		printVector(v1);
		printVector(v2); 
		printVector(v3);
		printVector(v4);
		printVector(v5);
		v4.push_back(3);
		printVector(v4);
		print(v4.capacity());
		v4 = v3;
		printVector(v4);
		print(v4.capacity());
		v4 = v5;
		printVector(v4);
		print(v4.capacity());
	}

	std::cout << "\tElement access\n";

	{
		vector<int>	v;

		int *i = &v.front(); print(i);
		i = &v.back(); print(i);
		// print(v[0]); segfault
		try { print(v.at(0)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		print(v.front());
		print(v.back());
		try { print(v.at(0)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v.at(4)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v[0]); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }	
	}

	std::cout << "\tIterators\n";

	{
		vector<int>	v;

		vector<int>::iterator					begin;
		vector<int>::iterator					end;
		
		// vector<int>::reverse_iterator		rbegin;
		// vector<int>::reverse_iterator		rend;
		// const vector<int>::iterator			cbegin;
		// const vector<int>::iterator			cend;
		// const vector<int>::reverse_iterator	crbegin;
		// const vector<int>::reverse_iterator	crend;

		begin = v.begin();
		end = v.end();

		// rbegin = v.rbegin();
		// rend = v.rend();
		// cbegin = v.cbegin();
		// cend = v.cend();
		// crbegin = v.crbegin();
		// crend = v.crend();


		print(*(void **)&begin);
		print(*(void **)&end);		

		// print(*(void **)&rbegin);
		// print(*(void **)&rend);
		// print(*(void **)&cbegin);
		// print(*(void **)&cend);		
		// print(*(void **)&crbegin);
		// print(*(void **)&crend);

		for (int i = 0; i < 10; i++)
			v.push_back(i);

		begin = v.begin();
		end = v.end();

		// rbegin = v.rbegin();
		// rend = v.rend();
		// cbegin = v.cbegin();
		// cend = v.cend();
		// crbegin = v.crbegin();
		// crend = v.crend();

		print(*(void **)&begin);
		print(*(void **)&end);
	
		// print(*(void **)&rbegin);
		// print(*(void **)&rend);
		// print(*(void **)&cbegin);
		// print(*(void **)&cend);
		// print(*(void **)&crbegin);
		// print(*(void **)&crend);
	
		print(*begin);
		print(*--end);

	}

	std::cout << "\tCapacity\n";

	{
		vector<int>	v;

		printVector(v);
		v.push_back(42);
		printVector(v);
		v.pop_back();
		printVector(v);
	}

	std::cout << "\tModifiers\n";

	{
		vector<int>	v;

		v.push_back(-42);
		vector<int>::iterator	begin = v.begin(); (void)begin;
		v.insert(v.begin(), array.begin(), array.end());
		printVector(v);
	}
}
