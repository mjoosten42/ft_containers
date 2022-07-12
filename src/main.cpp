#include <iostream>
#include "vector.hpp"

using namespace ft;

#define PRINT(x) std::cout << x << std::endl;

int	main() {
	vector<int>	v;

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
	}
}