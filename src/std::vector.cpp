#include <iostream>
#include <vector>

#define PRINT(x) std::cout << x << std::endl;

int	main() {
	std::vector<int>			v;

	{
	std::vector<int>::iterator	begin;
	std::vector<int>::iterator	end;

	begin = v.begin();
	end = v.end();
	PRINT(*(void **)&begin);
	PRINT(*(void **)&end);
	PRINT(v.capacity());
	PRINT(v.size());
	v.push_back(42);
	}



}