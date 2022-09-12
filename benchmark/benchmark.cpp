#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch_amalgamated.hpp"

#ifdef STD
	#include <vector>
	#include <stack>
	#include <map>
	#include <set>
	namespace ft = std;
#else
	#include "vector.hpp"
	#include "stack.hpp"
	#include "map.hpp"
	#include "set.hpp"
#endif

#include <cstdlib>
#include <string>

#define COUNT 1 << 8

TEST_CASE( "vector", "[vector]") {
	ft::vector<std::string>	v;

	BENCHMARK( "push_back" ) {
		for (int i = 0; i < COUNT; i++)
			v.push_back(std::string(i, '-'));
		return v.empty();
	};

	BENCHMARK( "pop_back" ) {
		while (v.size())
			v.pop_back();
		return v.empty();
	};

	BENCHMARK( "insert" ) {
		for (int i = 0; i < COUNT; i++)
			v.insert(v.begin() + i / 2, std::string(i, '-'));
		return v.empty();
	};

	BENCHMARK( "erase" ) {
		for (int i = v.size(); i; i--)
			v.erase(v.begin() + i / 2);
		return v.empty();
	};

	BENCHMARK( "resize" ) {
		for (int i = 0; i < COUNT; i++)
			v.resize(i);
		return v.empty();
	};
}

TEST_CASE( "stack", "[stack]") {
	ft::stack<std::string>	s;

	BENCHMARK( "push" ) {
		for (int i = 0; i < COUNT; i++)
			s.push(std::string(i, '-'));
		return s.empty();
	};

	BENCHMARK( "pop" ) {
		while (s.size())
			s.pop();
		return s.empty();
	};
}

TEST_CASE( "map", "[map]") {
	ft::map<int, std::string>	m;

	BENCHMARK( "insert" ) {
		for (int i = 0; i < COUNT; i++)
			m[i] = std::string(i, '-');
		return m.empty();
	};

	BENCHMARK( "erase" ) {
		while (m.size())
			m.erase(m.begin());
		return m.empty();
	};
}

TEST_CASE( "set", "[set]") {
	ft::set<std::string>	s;

	BENCHMARK( "insert" ) {
		for (int i = 0; i < COUNT; i++)
			s.insert(std::string(i, '-'));
		return s.empty();
	};

	BENCHMARK( "erase" ) {
		while (s.size())
			s.erase(s.begin());
		return s.empty();
	};
}
