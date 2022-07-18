#include <iostream>

#ifdef STD
	#include <vector>
	using std::vector;
#else
	#include "vector.hpp"
	using ft::vector;
#endif

#define PRINT(x) std::cout << x << std::endl;

int	main() {

	vector<int>	v;

	//	Member types
	{
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

	{
		vector<int>::iterator	begin;
		vector<int>::iterator	end;

		begin = v.begin();
		end = v.end();
		PRINT(*(void **)&begin);
		PRINT(*(void **)&end);
		PRINT(v.capacity());
		PRINT(v.size());
		v.push_back(42);
		v.push_back(42);
		v.push_back(42);
		begin = v.begin();
		end = v.end();
		PRINT(*(void **)&begin);
		PRINT(*(void **)&end);
		PRINT(v.capacity());
		PRINT(v.size());
		v.pop_back();
		v.pop_back();
		v.pop_back();
	}

	//	Element access
	{
		PRINT(v.size());
		v.push_back(42);
		PRINT(v.front());
		PRINT(v.back());
		PRINT(v.at(0));
		try { PRINT(v.at(1)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { PRINT(v[-1]); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
	}
}
