#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2.hpp"

#ifdef STD
	#include <stack>
	namespace ft = std;
#else
	#include "stack.hpp"
#endif

#include "myInt.hpp"
#include "myAllocator.hpp"
#include <queue>

TEMPLATE_TEST_CASE( "stack", "[int][myInt]", int, myInt ) {

	typedef ft::vector<TestType>		Vec;
	typedef ft::stack<TestType, Vec> 	Stack;
	typedef std::deque<TestType>		Deque;

	Vec	v;

	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	SECTION( "typedefs" ) {
		static_assert(std::is_same<typename Stack::container_type, Vec>::value, "container_type");
		static_assert(std::is_same<typename Stack::value_type, TestType>::value, "value_type");
		static_assert(std::is_same<typename Stack::size_type, typename Vec::size_type>::value, "size_type");
		static_assert(std::is_same<typename Stack::reference, typename Vec::reference>::value, "reference");
		static_assert(std::is_same<typename Stack::const_reference, typename Vec::const_reference>::value, "const_reference");
	}

	SECTION( "default" ) {
		Stack	s;

		REQUIRE( s.empty());

		Stack	t(v);

		REQUIRE( t.size() == v.size());
		REQUIRE( t.top() == 3);
	}

	SECTION( "custom container" ) {
		ft::stack<TestType, Deque>	u;

		REQUIRE( u.empty());
		u.push(42);
		REQUIRE( u.top() == 42);
		u.pop();
		REQUIRE( u.empty());
	}

	SECTION( "copy constructor" ) {
		Stack	s(v);
		Stack	t(s);

		REQUIRE( t == s);
	}

	SECTION( "copy assignment" ) {
		Stack	s(v);
		Stack	t;

		t.push(42);
		t = s;
		REQUIRE( t == s);
	}

	SECTION(" top" ) {
		Stack	s;

		s.push(42);
		REQUIRE( s.top() == 42);

		s.push(84);
		REQUIRE( s.top() == 84);

		s.pop();
		REQUIRE( s.top() == 42);

		s.top() = 7;
		REQUIRE( s.top() == 7);
	}

	SECTION( "empty, size" ) {
		Stack	s;

		REQUIRE(s.empty());
		REQUIRE( s.size() == 0);

		s.push(42);
		REQUIRE_FALSE(s.empty());
		REQUIRE( s.size() == 1);

		s.top() = 7;
		REQUIRE_FALSE(s.empty());
		REQUIRE( s.size() == 1);

		s.pop();
		REQUIRE(s.empty());
		REQUIRE( s.size() == 0);
	}

	SECTION( "comparison" ) {
		Stack	s(v);
		Stack	t(v);

		REQUIRE 		( t == s);
	
		REQUIRE			( s == t);
		REQUIRE_FALSE	( s != t);
		REQUIRE_FALSE	( s <  t);
		REQUIRE			( s <= t);
		REQUIRE_FALSE	( s >  t);
		REQUIRE			( s >= t);

		s.top() = -1;

		REQUIRE_FALSE	( s == t);
		REQUIRE			( s != t);
		REQUIRE			( s <  t);
		REQUIRE			( s <= t);
		REQUIRE_FALSE	( s >  t);
		REQUIRE_FALSE	( s >= t);
	
		s.top() = 4;

		REQUIRE_FALSE	( s == t);
		REQUIRE			( s != t);
		REQUIRE_FALSE	( s <  t);
		REQUIRE_FALSE	( s <= t);
		REQUIRE			( s >  t);
		REQUIRE			( s >= t);
	}
}
