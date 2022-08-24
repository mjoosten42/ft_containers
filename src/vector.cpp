#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2.hpp"

#include <array>
#include <set>
#include <sstream>

// #include "vector.hpp"
#include <vector>
namespace ft = std;

#include "myInt.hpp"
#include "myAllocator.hpp"

TEMPLATE_TEST_CASE( "vector", "", int, myInt ) {
	typedef ft::vector<TestType> vec;

	const myAllocator<TestType>		myAlloc;
	const std::array<TestType, 3>	array = { 1, 2, 3 };
	const std::set<TestType>		set = { 4, 5, 6 };
	std::istringstream				iss("7 8 9");

	SECTION( "constructors ") {
		SECTION( "default " ) {
			vec v;

			REQUIRE( v.size() == 0 );
			REQUIRE( v.empty() == true);
		}

		SECTION( "custom allocator" ) {
			ft::vector<TestType, myAllocator<TestType>> v(myAlloc);

			REQUIRE( v.size() == 0 );
			REQUIRE( v.empty() == true);
		}

		SECTION( "value" ) {

			vec v(3, 42);

			REQUIRE( v.capacity() >= 3);
			REQUIRE( v.empty() == false);
			REQUIRE( v.size() == 3);

			for (int i = 0; i < 3; i++) {
				REQUIRE( v[i] == 42);
			}

			vec w(0, 42);

			REQUIRE( w.capacity() >= 0);
			REQUIRE( w.empty() == true);
			REQUIRE( w.size() == 0);
		}
	
		SECTION( "default value" ) {
			vec v(3);

			REQUIRE( v.capacity() >= 3);
			REQUIRE( v.empty() == false);
			REQUIRE( v.size() == 3);
			for (int i = 0; i < 3; i++)
				REQUIRE( v[i] == TestType());
		}

		SECTION( " range" ) {
			vec v(3, 42);

			vec	a(v.begin(), v.end());

			REQUIRE( a.capacity() >= 3);
			REQUIRE( a.empty() == false);
			REQUIRE( a.size() == 3);
			for (int i = 0; i < 3; i++)
				REQUIRE( a[i] == 42);
			
			vec	b(array.begin(), array.end());

			REQUIRE( b.capacity() >= 3);
			REQUIRE( b.empty() == false);
			REQUIRE( b.size() == 3);
			REQUIRE( b[0] == 1);
			REQUIRE( b[1] == 2);
			REQUIRE( b[2] == 3);
		
			vec	c(set.begin(), set.end());

			REQUIRE( c.capacity() >= 3);
			REQUIRE( c.empty() == false);
			REQUIRE( c.size() == 3);
			REQUIRE( c[0] == 4);
			REQUIRE( c[1] == 5);
			REQUIRE( c[2] == 6);

			vec	d((std::istream_iterator<TestType>(iss)), std::istream_iterator<TestType>());
		
			REQUIRE( d.capacity() >= 3);
			REQUIRE( d.empty() == false);
			REQUIRE( d.size() == 3);
			REQUIRE( d[0] == 7);
			REQUIRE( d[1] == 8);
			REQUIRE( d[2] == 9);
		}

		SECTION( " copy ") {
			vec v(3, 42);
			vec w(v);

			REQUIRE( v.capacity() >= 3);
			REQUIRE( v.empty() == false);
			REQUIRE( v.size() == 3);

			for (int i = 0; i < 3; i++) {
				REQUIRE( v[i] == 42);
			}
		
			REQUIRE( w.capacity() >= 3);
			REQUIRE( w.empty() == false);
			REQUIRE( w.size() == 3);

			for (int i = 0; i < 3; i++) {
				REQUIRE( w[i] == 42);
			}

			vec x;
			vec y(x);

			REQUIRE( x.size() == 0 );
			REQUIRE( x.empty() == true);

			REQUIRE( y.size() == 0 );
			REQUIRE( y.empty() == true);
		}
	}

	SECTION( "copy assigment ") {
		vec	v(3, 42);

		vec	u(1);

		u = v;

		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.empty() == false);
		REQUIRE( v.size() == 3);
		for (int i = 0; i < 3; i++)
			REQUIRE( v[i] == 42);

		REQUIRE( u.capacity() >= 3);
		REQUIRE( u.empty() == false);
		REQUIRE( u.size() == 3);
		for (int i = 0; i < 3; i++)
			REQUIRE( u[i] == 42);

		REQUIRE( v.data() != u.data());
	}

	SECTION( "assign") {
		vec	v;

		v.assign(3, 42);
	
		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.empty() == false);
		REQUIRE( v.size() == 3);
		for (int i = 0; i < 3; i++)
			REQUIRE( v[i] == 42);

		v.assign(0, 42);
	
		REQUIRE( v.empty() == true);
		REQUIRE( v.size() == 0);

		v.assign(array.begin(), array.end());

		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.empty() == false);
		REQUIRE( v.size() == 3);
		REQUIRE( v[0] == 1);
		REQUIRE( v[1] == 2);
		REQUIRE( v[2] == 3);

		v.assign(set.begin(), set.end());
	
		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.empty() == false);
		REQUIRE( v.size() == 3);
		REQUIRE( v[0] == 4);
		REQUIRE( v[1] == 5);
		REQUIRE( v[2] == 6);

		iss.str("7 8 9");
	
		v.assign((std::istream_iterator<TestType>(iss)), std::istream_iterator<TestType>());
	
		REQUIRE( v.capacity() >= 3);
		REQUIRE( v.empty() == false);
		REQUIRE( v.size() == 3);
		REQUIRE( v[0] == 7);
		REQUIRE( v[1] == 8);
		REQUIRE( v[2] == 9);
	}

	SECTION(" allocator") {
		vec	v;
	
		REQUIRE( v.get_allocator() == std::allocator<TestType>());
	}

	SECTION(" element access") {
		const	vec	v(array.begin(), array.end());
				vec	u(set.begin(), set.end());		

		SECTION(" at") {
			REQUIRE_NOTHROW( v.at(0) == 1);
			REQUIRE_NOTHROW( u.at(2) = 6 == 6);
			REQUIRE_THROWS( v.at(3) == 0);
			REQUIRE_THROWS( u.at(-1) == 0);
			REQUIRE_THROWS( v.at(std::numeric_limits<typename vec::size_type>::max()) == 0);
		}

		SECTION(" operator[]") {
			REQUIRE( v[0] == 1);
			REQUIRE( u[2] == 6);
		}

		SECTION(" front") {
			REQUIRE( v.front() == 1);
			REQUIRE( u.front() == 4);
		}

		SECTION(" back") {
			REQUIRE( v.back() == 3);
			REQUIRE( u.back() == 6);
		}

		SECTION(" data") {
			REQUIRE( *v.data() == v[0]);
			REQUIRE( *u.data() == u[0]);
		}
	}

	SECTION(" iterators") {
		const	vec	v(array.begin(), array.end());
				vec	u(set.begin(), set.end());	
	
		typename vec::const_iterator cit = v.begin();
		typename vec::iterator it = u.begin();
		typename vec::const_iterator cite = v.end();
		typename vec::iterator ite = u.end();

		while (it != ite) {
			REQUIRE( (it == ite) == (cit == cite));
			REQUIRE( *it - 3 == *cit);
			it++;
			cit++;
		}
	}

}
