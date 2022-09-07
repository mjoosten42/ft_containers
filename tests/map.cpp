#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2.hpp"

#if 0
	#include <map>
	namespace ft = std;
#else
	#include "map.hpp"
#endif

#include <string>
#include "myInt.hpp"
#include "myAllocator.hpp"

struct myCompare {
	bool	operator()(int lhs, int rhs) const {
		return lhs % 10 < rhs % 10;
	}
};

TEMPLATE_TEST_CASE( "map", "[map]", int, myInt ) {
	typedef typename ft::map<TestType, std::string>	Map;
	typedef typename Map::iterator					Iter;

	const ft::pair<TestType, std::string>	array[] = {
		{ 8,  "h" },
		{ 3,  "c" },
		{ 1,  "a" },
		{ 6,  "f" },
		{ 4,  "d" },
		{ 7,  "g" },
		{ 10, "b" },
		{ 14, "f" },
		{ 13, "e" }
	};
	const uint	size = sizeof(array) / sizeof(array[0]);

	SECTION( "typedefs" ) {
		static_assert(std::is_same<typename Map::key_type, TestType>::value, "");
		static_assert(std::is_same<typename Map::mapped_type, std::string>::value, "");
		static_assert(std::is_same<typename Map::value_type, ft::pair<const TestType, std::string> >::value, "");
		static_assert(std::is_unsigned<typename Map::size_type>::value, "");
		static_assert(std::is_signed<typename Map::difference_type>::value, "");
		static_assert(std::is_same<typename Map::key_compare, std::less<TestType> >::value, "");
		static_assert(std::is_same<typename Map::allocator_type, std::allocator<ft::pair<const TestType, std::string> > >::value, "");
		static_assert(std::is_same<typename Map::reference, typename Map::value_type&>::value, "");
		static_assert(std::is_same<typename Map::const_reference, const typename Map::value_type&>::value, "");
		static_assert(std::is_same<typename Map::pointer, typename Map::value_type*>::value, "");
		static_assert(std::is_same<typename Map::const_pointer, const typename Map::value_type*>::value, "");
	}

	SECTION( "Constructors") {
		SECTION( "default") {
			Map	m;

			REQUIRE( m.empty());
		}

		SECTION( "custom compare and allocator" ) {
			myAllocator<ft::pair<const TestType, std::string> >			myAlloc(42);
			ft::map<TestType, std::string, myCompare,
				myAllocator<ft::pair<const TestType, std::string> > >	m(myCompare(), myAlloc);

			REQUIRE( m.get_allocator() == myAllocator<ft::pair<const TestType, std::string> >());
			REQUIRE( m.max_size() == 42);
			
			m[2] = "A";
			m[11] = "B";
			REQUIRE( m.begin()->second == "B");
		}
	
		SECTION( "InputIt") {
			Map	m(array, array + size);

			REQUIRE_FALSE( m.empty());
			REQUIRE( m.size() == size);
		}

		SECTION( "copy constructor") {
			Map	m(array, array + size);
			Map	n(m);

			REQUIRE( n == m);
		}
	}

	SECTION( "copy assignment" ) {
		Map m(array, array + size);
		Map n(array + 2, array + 5);
		Map	o;

		n = m;
		REQUIRE( n == m);
		n = o;
		REQUIRE( n == o);
		o = m;
		REQUIRE( o == m);
	}

	SECTION( "get_allocator" ) {
		Map	m;

		REQUIRE( m.get_allocator() == std::allocator<ft::pair<TestType, std::string> >());
	}

	SECTION( "Element access") {
		SECTION( "at" ) {
			Map	m(array, array + size);

			REQUIRE_THROWS_AS( m.at(-1), std::out_of_range);
			REQUIRE_THROWS_AS( m.at(0), std::out_of_range);
			REQUIRE_THROWS_AS( m.at(5), std::out_of_range);
			REQUIRE_NOTHROW( m.at(1) == "a");
			REQUIRE_NOTHROW( m.at(7) == "g");
			REQUIRE_NOTHROW( m.at(14) == "n");

			m.at(3) = "C";
			REQUIRE_NOTHROW( m.at(3) == "C");
		}

		SECTION( "operator []" ) {
			Map	m(array, array + size);

			REQUIRE( m[3] == "c");
			REQUIRE( m[13] == "e");
			m[5] = "E";
			m[10] = "J";
			m[26] = "Z";
			REQUIRE( m.size() == 11);
			REQUIRE( m[5] == "E");
			REQUIRE( m[10] == "J");
			REQUIRE( m[26] == "Z");
			REQUIRE( m[0] == "");
		}
	}
	
	SECTION( "Iterators" ) {
		Map	m(array, array + size);
	
		typename Map::iterator	it = m.begin();
		typename Map::iterator	ite = m.end();

		it++;
		it->second = "C";
		REQUIRE( m[3] == "C");
		REQUIRE( it != ite);

		typename Map::const_iterator	cit = m.begin();
		typename Map::const_iterator	cite = m.end();

		Map	n(cit, cite);
		REQUIRE( m == n);

		typename Map::reverse_iterator	rit = m.rbegin();
		typename Map::reverse_iterator	rite = m.rend();

		for (; rit != rite; rit++)
			rit->second = "42";

		typename Map::const_reverse_iterator	crit = m.rbegin();
		typename Map::const_reverse_iterator	crite = m.rend();

		for (; crit != crite; crit++)
			REQUIRE( crit->second == "42");
	}

	SECTION( "Capacity" ) {
		Map	m;
		Map n(array, array + size);

		REQUIRE( m.size() == 0);
		REQUIRE( m.empty());

		m.insert(array, array + size);
		REQUIRE( m.size() == size);
		REQUIRE_FALSE(m.empty());

		n = m;
		REQUIRE( n.size() == size);
		REQUIRE_FALSE(n.empty());
	
		m.clear();
		REQUIRE( m.size() == 0);
		REQUIRE( m.empty());

		REQUIRE( m.max_size() > 0);
	}

	SECTION( "Modifiers" ) {
		SECTION( "clear" ) {
			Map	m;

			m.clear();
			REQUIRE( m.empty());

			m.insert(array, array + size);
			m.clear();
			REQUIRE( m.empty());
		}

		SECTION( "insert" ) {
			Map						m;
			Map						n;
			ft::pair<Iter, bool>	p;
			Iter					it;

			p = m.insert(ft::make_pair(5, "E"));
			REQUIRE( p.first->second == "E");
			REQUIRE( p.second == true);

			p = m.insert(ft::make_pair(5, "E"));
			REQUIRE( p.first->second == "E");
			REQUIRE( p.second == false);

			m.insert(array, array + size);
			REQUIRE( m.size() == size + 1);
		
			it = m.insert(m.begin(), ft::make_pair(12, "K"));
			REQUIRE( it->second == "K");
		
			it = m.insert(m.begin(), ft::make_pair(12, "K"));
			REQUIRE( it->second == "K");

			n.insert(++m.begin(), --m.end());
			REQUIRE( n.size() == size);
			REQUIRE( n != m);
		}

		SECTION( "erase" ) {
			Map	m(array, array + size);

			m.erase(m.begin());
			REQUIRE( m.size() == size - 1);
			REQUIRE( m.find(1) == m.end());

			m.erase(m.begin(), m.end());
			REQUIRE( m.empty());

			REQUIRE( m.erase(1) == 0);
			m[1] = "A";
			REQUIRE( m.erase(1) == 1);
		}

		SECTION( "swap" ) {
			Map	m(array + 1, array + 7);
			Map	n(++m.rbegin(), --m.rend());

			m.swap(n);
			REQUIRE( m.size() == m.size());
			REQUIRE( m[3] == "c");

			Map().swap(m);
			REQUIRE( m.empty());
		}
	}

	SECTION( "Lookup" ) {
		Map	m;

		REQUIRE( m.count(1) == 0);
		REQUIRE( m.find(1) == m.end());
		m[1] = "A";
		REQUIRE( m.count(1) == 1);
		REQUIRE( m.find(1)->second == "A");
		m[1] = "B";
		REQUIRE( m.count(1) == 1);
		REQUIRE( m.find(1)->second == "B");

		SECTION( "ranges") {
			Map						m(array, array + size);
			ft::pair<Iter, Iter>	p;

			p = m.equal_range(3);
			for (; p.first != p.second; p.first++)
				REQUIRE( p.first->second == "c");

			p.first = m.lower_bound(5);
			p.second = m.upper_bound(12);
			uint i = 0;
			for (; p.first != p.second; p.first++)
				i++;
			REQUIRE( i == 4);
		}
	}

	SECTION( "observers" ) {
		Map	m;

		REQUIRE( m.key_comp()(1, 2));
		REQUIRE_FALSE( m.key_comp()(1, 1));
		REQUIRE_FALSE( m.key_comp()(2, 1));
	
		REQUIRE( m.value_comp()(ft::make_pair(1, "a"), ft::make_pair(2, "a")));
		REQUIRE_FALSE( m.value_comp()(ft::make_pair(1, "a"), ft::make_pair(1, "a")));
		REQUIRE_FALSE( m.value_comp()(ft::make_pair(1, "a"), ft::make_pair(1, "b")));
	}

	SECTION( "comparisons" ) {
		Map	m(array, array + size);
		Map	n(array, array + size);
	
		REQUIRE 		( n == m);
	
		REQUIRE			( m == n);
		REQUIRE_FALSE	( m != n);
		REQUIRE_FALSE	( m <  n);
		REQUIRE			( m <= n);
		REQUIRE_FALSE	( m >  n);
		REQUIRE			( m >= n);

		n[15] = "B";

		REQUIRE_FALSE	( m == n);
		REQUIRE			( m != n);
		REQUIRE			( m <  n);
		REQUIRE			( m <= n);
		REQUIRE_FALSE	( m >  n);
		REQUIRE_FALSE	( m >= n);
	
		m[16] = "C";

		REQUIRE_FALSE	( m == n);
		REQUIRE			( m != n);
		REQUIRE_FALSE	( m <  n);
		REQUIRE_FALSE	( m <= n);
		REQUIRE			( m >  n);
		REQUIRE			( m >= n);
	}

	SECTION( "std::swap" ) {
		Map	m(array, array + size);
		Map	n(array + 3, array + 7);

		std::swap(m, n);
		REQUIRE( m.begin()->second == "d");
		REQUIRE( n.begin()->second == "a");
	}
}
