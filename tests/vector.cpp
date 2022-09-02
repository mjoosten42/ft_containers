#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2.hpp"

#include <array>
#include <set>
#include <sstream>

#if 1
	#include <vector>
	namespace ft = std;
#else
	#include "vector.hpp"
#endif

#include "myInt.hpp"
#include "myAllocator.hpp"

TEMPLATE_TEST_CASE( "vector", "[int][myInt]", int, myInt ) {
	typedef ft::vector<TestType> 			Vec;
	typedef	std::istream_iterator<TestType>	InputIt;

	const myAllocator<TestType>		myAlloc(16);
	const std::array<TestType, 3>	array = { 1, 2, 3 };
	const std::set<TestType>		set = { 4, 5, 6 };
	std::istringstream				iss("7 8 9");

	REQUIRE(myit == sysit);
	SECTION( "constructors ") {
		SECTION( "default " ) {
			Vec v;

			REQUIRE( v.size() == 0 );
			REQUIRE( v.empty());
		}

		SECTION( "custom allocator" ) {
			ft::vector<TestType, myAllocator<TestType>> v(myAlloc);

			REQUIRE( v.size() == 0 );
			REQUIRE( v.empty());

			REQUIRE( v.max_size() == 16);
		}

		SECTION( "value" ) {

			Vec v(3, 42);

			REQUIRE( v.capacity() >= 3);
			REQUIRE( v.size() == 3);
			REQUIRE_FALSE( v.empty());

			for (int i = 0; i < 3; i++) {
				REQUIRE( v[i] == 42);
			}

			Vec w(0, 42);

			REQUIRE( w.capacity() >= 0);
			REQUIRE( w.size() == 0);
			REQUIRE( w.empty());
		}
	
		SECTION( "default value" ) {
			Vec v(3);

			REQUIRE( v.capacity() >= 3);
			REQUIRE( v.size() == 3);
			REQUIRE_FALSE( v.empty());
			for (int i = 0; i < 3; i++)
				REQUIRE( v[i] == TestType());
		}

		SECTION( " range" ) {
			Vec v(3, 42);

			Vec	a(v.begin(), v.end());

			REQUIRE( a.capacity() >= 3);
			REQUIRE( a.size() == 3);
			REQUIRE( !a.empty());
			for (int i = 0; i < 3; i++)
				REQUIRE( a[i] == 42);
			
			Vec	b(array.begin(), array.end());

			REQUIRE( b.capacity() >= 3);
			REQUIRE( b.size() == 3);
			REQUIRE( b[0] == 1);
			REQUIRE( b[1] == 2);
			REQUIRE( b[2] == 3);
		
			Vec	c(set.begin(), set.end());

			REQUIRE( c.capacity() >= 3);
			REQUIRE( c.size() == 3);
			REQUIRE( c[0] == 4);
			REQUIRE( c[1] == 5);
			REQUIRE( c[2] == 6);

			Vec	d((InputIt(iss)), InputIt());
		
			REQUIRE( d.capacity() >= 3);
			REQUIRE( d.size() == 3);
			REQUIRE( d[0] == 7);
			REQUIRE( d[1] == 8);
			REQUIRE( d[2] == 9);
		}

		SECTION( " copy ") {
			Vec v(3, 42);
			Vec w(v);

			REQUIRE( v.capacity() >= 3);
			REQUIRE( v.size() == 3);

			for (int i = 0; i < 3; i++) {
				REQUIRE( v[i] == 42);
			}
		
			REQUIRE( w.capacity() >= 3);
			REQUIRE( w.size() == 3);

			for (int i = 0; i < 3; i++) {
				REQUIRE( w[i] == 42);
			}

			Vec x;
			Vec y(x);

			REQUIRE( x.size() == 0 );
			REQUIRE( x.empty());

			REQUIRE( y.size() == 0 );
			REQUIRE( y.empty());
		}
	}

	SECTION( "copy assigment ") {
		Vec	v(3, 42);
		Vec	u(1);

		u = v;

		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.size() == 3);
		for (int i = 0; i < 3; i++)
			REQUIRE( v[i] == 42);

		REQUIRE( u.capacity() >= 3);
		REQUIRE( u.size() == 3);
		for (int i = 0; i < 3; i++)
			REQUIRE( u[i] == 42);

		REQUIRE( v.data() != u.data());
	}

	SECTION( "assign") {
		Vec	v;

		v.assign(3, 42);
	
		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.size() == 3);
		for (int i = 0; i < 3; i++)
			REQUIRE( v[i] == 42);

		v.assign(0, 42);
	
		REQUIRE( v.size() == 0);
		REQUIRE( v.empty());

		v.assign(array.begin(), array.end());

		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.size() == 3);
		REQUIRE( v[0] == 1);
		REQUIRE( v[1] == 2);
		REQUIRE( v[2] == 3);

		v.assign(set.begin(), set.end());
	
		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.size() == 3);
		REQUIRE( v[0] == 4);
		REQUIRE( v[1] == 5);
		REQUIRE( v[2] == 6);

		iss.str("7 8 9");
	
		v.assign((std::istream_iterator<TestType>(iss)), std::istream_iterator<TestType>());
	
		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.size() == 3);
		REQUIRE( v[0] == 7);
		REQUIRE( v[1] == 8);
		REQUIRE( v[2] == 9);
	}

	SECTION( "allocator" ) {
		Vec												v;
		ft::vector<TestType, myAllocator<TestType> >	w(myAlloc);
	
		REQUIRE( v.get_allocator() == std::allocator<TestType>());
		REQUIRE( w.get_allocator() == myAlloc);
	}

	SECTION( "element access" ) {
		const	Vec	v(array.begin(), array.end());
				Vec	u(set.begin(), set.end());		

		SECTION(" at") {
			REQUIRE_NOTHROW( v.at(0) == 1);
			REQUIRE_NOTHROW( u.at(2) = 6 == 6);
			REQUIRE_THROWS( v.at(3));
			REQUIRE_THROWS( u.at(-1));
			REQUIRE_THROWS( v.at(std::numeric_limits<typename Vec::size_type>::max()));
		}

		SECTION( "operator[]") {
			REQUIRE( v[0] == 1);
			REQUIRE( u[2] == 6);
		}

		SECTION( "front") {
			REQUIRE( v.front() == 1);
			REQUIRE( u.front() == 4);
		}

		SECTION( "back") {
			REQUIRE( v.back() == 3);
			REQUIRE( u.back() == 6);
		}

		SECTION( "data") {
			REQUIRE( *v.data() == v[0]);
			REQUIRE( *u.data() == u[0]);
		}

		u.pop_back();
		u.pop_back();

		REQUIRE (u.front() == u.back());
	}

	SECTION( "iterators" ) {
		const	Vec	v(array.begin(), array.end());
				Vec	u(set.begin(), set.end());	
	
		typename Vec::iterator it = u.begin();
		typename Vec::iterator ite = u.end();
		typename Vec::const_iterator cit = v.begin();
		typename Vec::const_iterator cite = v.end();

		while (it != ite) {
			REQUIRE( (it == ite) == (cit == cite));
			REQUIRE( *it - 3 == *cit);
			it++;
			cit++;
		}
	
		typename Vec::reverse_iterator rit = u.rbegin();
		typename Vec::reverse_iterator rite = u.rend();
		typename Vec::const_reverse_iterator rcit = v.rbegin();
		typename Vec::const_reverse_iterator rcite = v.rend();

		while (rit != rite) {
			REQUIRE( (rit == rite) == (rcit == rcite));
			REQUIRE( *rit - 3 == *rcit);
			rit++;
			rcit++;
		}

		Vec	w;

		REQUIRE( w.begin() == w.end());

		w.push_back(42);
		w.pop_back();

		REQUIRE( w.begin() == w.end());
	}

	SECTION ("Capacity" ) {
		SECTION( "empty and size" ) {	
			Vec	v;

			REQUIRE( v.size() == 0);
			REQUIRE( v.empty());

			v.push_back(42);

			REQUIRE( v.size() == 1);
			REQUIRE_FALSE( v.empty());

			v.pop_back();

			REQUIRE( v.size() == 0);
			REQUIRE( v.empty());

			v.reserve(3);

			REQUIRE( v.size() == 0);
			REQUIRE( v.empty());
		}

		SECTION( "max_size" ) {
			Vec	v;

			REQUIRE ( v.max_size() > 0);

			ft::vector<TestType, myAllocator<TestType> >	w(myAlloc);

			REQUIRE ( w.max_size() == myAlloc.max_size());
		}

		SECTION( "reserve" ) {
			Vec			v;
			TestType*	data;

			v.reserve(10);
			data = v.data();
			for (int i = 0; i < 10; i++)
				v.push_back(i);
			
			REQUIRE( v.data() == data);

			v.reserve(5);
			
			REQUIRE( v.capacity() >= 10);
		}

		SECTION( "capacity" ) {
			Vec	v;

			v.reserve(10);

			REQUIRE ( v.capacity() >= 10);

			auto it = v.begin() + 3;

			for (int i = 0; i < 10; i++)
				v.push_back(i);

			REQUIRE ( *it == 3);
			REQUIRE ( v.capacity() >= 10);

			v.push_back(42);

			REQUIRE ( v.capacity() >= 11);
		}
	}

	SECTION( "modifiers" ) {
		SECTION( "clear" ) {
			Vec		v;
			uint	cap;

			v.clear();

			REQUIRE ( v.size() == 0);

			v.assign(array.begin(), array.end());
			cap = v.capacity();
			v.clear();

			REQUIRE ( v.size() == 0);
			REQUIRE ( v.capacity() == cap);

			v.push_back(1);
			v.push_back(2);
			typename Vec::iterator it = v.begin() + 1;
			v.push_back(3);
			REQUIRE (*it == 2);
		}

		SECTION( "insert" ) {
			Vec	v;
			Vec	out;

			v.insert(v.begin(), array.begin(), array.end());

			auto it = v.insert(v.begin(), 42);
			REQUIRE ( *it == 42);

			v.insert(v.begin() + 2, 2, 7);
			v.insert(v.begin() + 3, set.begin(), set.end());

			iss.str("7 8 9");
			v.insert(v.end(), InputIt(iss), InputIt());

			out.push_back(42);
			out.push_back(1);
			out.push_back(7);
			out.push_back(4);
			out.push_back(5);
			out.push_back(6);
			out.push_back(7);
			out.push_back(2);
			out.push_back(3);
			out.push_back(7);
			out.push_back(8);
			out.push_back(9);

			REQUIRE ( v == out);
		}

		SECTION( "erase" ) {
			Vec	v;


			auto it = v.erase(v.begin(), v.end());
			REQUIRE ( it == v.end());

			v.push_back(42);

			it = v.erase(v.begin());
			REQUIRE ( it == v.end());

			v.assign(array.begin(), array.end());
			v.insert(v.end(), set.begin(), set.end());

			v.erase(v.begin() + 1, v.end() - 1);
			v.erase(v.begin() + 1);

			REQUIRE ( v[0] == 1);
		}
		
		SECTION( " push_back" ) {
			Vec	v;

			v.push_back(0);
			v.push_back(1);

			REQUIRE ( v.capacity() >= 2);
			REQUIRE ( v.size() == 2);
			REQUIRE ( v[0] == 0);
			REQUIRE ( v[1] == 1);

			for (int i = 2; i < 10; i++)
				v.push_back(v[i - 2] + v[i - 1]);

			REQUIRE ( v[9] == 34);
		}

		SECTION( " pop_back" ) {
			Vec	v(set.begin(), set.end());

			for (int i = set.size(); i > 0; i--)
				v.pop_back();

			REQUIRE ( v.empty());
			REQUIRE ( v.capacity() >= set.size());
		}

		SECTION( "resize" ) {
			Vec	v;

			v.resize(10);

			REQUIRE ( v.size() == 10);
			REQUIRE ( v.capacity() >= 10);
			REQUIRE ( v[9] == TestType());

			v.resize(5);

			REQUIRE ( v.size() == 5);
			REQUIRE ( v.capacity() >= 10);

			v.resize(5, 7);

			REQUIRE ( v.size() == 5);
			REQUIRE ( v.capacity() >= 10);
			REQUIRE ( v[4] == TestType());

			v.resize(15, 42);

			REQUIRE ( v[14] == 42);
			REQUIRE ( v.size() == 15);
			REQUIRE ( v.capacity() >= 15);

			v.resize(0);

			REQUIRE ( v.empty());

			Vec	w;

			w.resize(0);

			REQUIRE ( w.empty());
		}

		SECTION( "swap" ) {
			Vec	v(array.begin(), array.end());
			Vec	w;

			TestType*	vdata = v.data();
			TestType*	wdata = w.data();

			v.swap(w);

			REQUIRE ( v.empty());
			REQUIRE ( w.size() == 3);
			REQUIRE ( w[2] == 3);
			REQUIRE ( v.data() == wdata);
			REQUIRE ( w.data() == vdata);

			w.reserve(10);
		
			Vec(w).swap(w);

			REQUIRE ( w.capacity() == w.size());
		}
	}

	SECTION( "other" ) {
		SECTION( "comparison" ) {
			const	Vec	v(array.begin(), array.end());
					Vec	w(array.begin(), array.end());

			REQUIRE 		( w == v);
		
			REQUIRE			( v == w);
			REQUIRE_FALSE	( v != w);
			REQUIRE_FALSE	( v <  w);
			REQUIRE			( v <= w);
			REQUIRE_FALSE	( v >  w);
			REQUIRE			( v >= w);

			w[2] = 4;

			REQUIRE_FALSE	( v == w);
			REQUIRE			( v != w);
			REQUIRE			( v <  w);
			REQUIRE			( v <= w);
			REQUIRE_FALSE	( v >  w);
			REQUIRE_FALSE	( v >= w);
		
			w[2] = 2;

			REQUIRE_FALSE	( v == w);
			REQUIRE			( v != w);
			REQUIRE_FALSE	( v <  w);
			REQUIRE_FALSE	( v <= w);
			REQUIRE			( v >  w);
			REQUIRE			( v >= w);
		}

		SECTION( "std::swap" ) {
			Vec	v(array.begin(), array.end());
			Vec	w(set.begin(), set.begin());

			TestType*	vdata = v.data();
			TestType*	wdata = w.data();

			std::swap(v, w);

			REQUIRE ( v.data() == wdata);
			REQUIRE ( w.data() == vdata);
		}
	}
}
