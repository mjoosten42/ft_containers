#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2.hpp"

#if 0
	#include <set>
	namespace ft = std;
#else
	#include "set.hpp"
#endif

#include <string>
#include "myInt.hpp"
#include "myAllocator.hpp"

struct myCompare {
	bool	operator()(int lhs, int rhs) const {
		return lhs % 10 < rhs % 10;
	}
};

TEMPLATE_TEST_CASE( "set", "[set]", int, myInt ) {
	typedef typename ft::set<TestType>	Set;
	typedef typename Set::iterator		Iter;

	const TestType	array[] = { 8, 3, 1, 6, 4, 7, 10, 14, 13 };
	const uint		size = sizeof(array) / sizeof(array[0]);

	SECTION( "typedefs" ) {
		static_assert(std::is_same<typename Set::key_type, TestType>::value, "key_type");
		static_assert(std::is_same<typename Set::value_type, TestType>::value, "value_type");
		static_assert(std::is_unsigned<typename Set::size_type>::value, "size_type");
		static_assert(std::is_signed<typename Set::difference_type>::value, "difference_type");
		static_assert(std::is_same<typename Set::key_compare, std::less<TestType> >::value, "key_compare");
		static_assert(std::is_same<typename ft::set<TestType, myCompare>::key_compare, myCompare>::value, "key_compare");
		static_assert(std::is_same<typename Set::value_compare, std::less<TestType> >::value, "custom value_compare");
		static_assert(std::is_same<typename ft::set<TestType, myCompare>::value_compare, myCompare>::value, "custom key_compare");
		static_assert(std::is_same<typename Set::allocator_type, std::allocator<TestType> >::value, "allocator_type");
		static_assert(std::is_same<typename ft::set<TestType, myCompare, myAllocator<TestType> >::allocator_type, myAllocator<TestType> >::value, "allocator_type");
		static_assert(std::is_same<typename Set::reference, TestType&>::value, "reference");
		static_assert(std::is_same<typename Set::const_reference, const TestType&>::value, "const_reference");
		static_assert(std::is_same<typename Set::pointer, TestType*>::value, "pointer");
		static_assert(std::is_same<typename Set::const_pointer, const TestType*>::value, "const_pointer");
	}

	SECTION( "Constructors") {
		SECTION( "default") {
			Set	s;

			REQUIRE( s.empty());
		}

		SECTION( "custom compare and allocator" ) {
			myAllocator<TestType>									myAlloc(42);
			ft::set<TestType, myCompare, myAllocator<TestType> >	s(myCompare(), myAlloc);

			REQUIRE( s.get_allocator() == myAllocator<TestType>());
			REQUIRE( s.max_size() == 42);
			
			s.insert(2);
			s.insert(11);
			REQUIRE( *s.begin() == 11);
		}
	
		SECTION( "InputIt") {
			Set	s(array, array + size);

			REQUIRE_FALSE( s.empty());
			REQUIRE( s.size() == size);
		}

		SECTION( "copy constructor") {
			Set	s(array, array + size);
			Set	t(s);

			REQUIRE( s == t);
		}
	}

	SECTION( "copy assignment" ) {
		Set	s(array, array + size);
		Set	t(array + 2, array + 5);
		Set	u;

		t = s;
		REQUIRE( t == s);
		t = u;
		REQUIRE( t == u);
		u = s;
		REQUIRE( u == s);
	}

	SECTION( "get_allocator" ) {
		Set														s;
		ft::set<TestType, myCompare, myAllocator<TestType> >	t;

		REQUIRE( s.get_allocator() == std::allocator<TestType>());
		REQUIRE( t.get_allocator() == myAllocator<TestType>());
	}

	SECTION( "Iterators" ) {
		Set	s(array, array + size);
	
		typename Set::iterator	it = s.begin();
		typename Set::iterator	ite = s.end();

		REQUIRE( it != ite);
	
		it++;
		REQUIRE( *it == 3);

		typename Set::const_iterator	cit = s.begin();
		typename Set::const_iterator	cite = s.end();

		Set	t(cit, cite);
		REQUIRE( s == t);

		typename Set::reverse_iterator	rit = s.rbegin();
		typename Set::reverse_iterator	rite = s.rend();

		REQUIRE(*std::find(rit, rite, 13) == 13);
	
		typename Set::const_reverse_iterator	crit = s.rbegin();
		typename Set::const_reverse_iterator	crite = s.rend();

		REQUIRE(*std::find(crit, crite, 13) == 13);
	}

	SECTION( "Capacity" ) {
		Set	s;
		Set	t(array, array + size);

		REQUIRE( s.size() == 0);
		REQUIRE( s.empty());

		s.insert(array, array + size);
		REQUIRE( s.size() == size);
		REQUIRE_FALSE(s.empty());

		t = s;
		REQUIRE( t.size() == size);
		REQUIRE_FALSE(t.empty());
	
		s.clear();
		REQUIRE( s.size() == 0);
		REQUIRE( s.empty());

		REQUIRE( s.max_size() > 0);
	}

	SECTION( "Modifiers" ) {
		SECTION( "clear" ) {
			Set	s;

			s.clear();
			REQUIRE( s.empty());

			s.insert(array, array + size);
			s.clear();
			REQUIRE( s.empty());
		}

		SECTION( "insert" ) {
			Set						s;
			Set						t;
			ft::pair<Iter, bool>	p;
			Iter					it;

			p = s.insert(5);
			REQUIRE( *p.first == 5);
			REQUIRE( p.second == true);

			p = s.insert(5);
			REQUIRE( *p.first == 5);
			REQUIRE( p.second == false);

			s.insert(array, array + size);
			REQUIRE( s.size() == size + 1);
		
			it = s.insert(s.begin(), 12);
			REQUIRE( *it == 12);
		
			it = s.insert(s.begin(), 12);
			REQUIRE( *it == 12);

			t.insert(++s.begin(), --s.end());
			REQUIRE( t.size() == size);
			REQUIRE( t != s);
		}

		SECTION( "erase" ) {
			Set	s(array, array + size);

			s.erase(s.begin());
			REQUIRE( s.size() == size - 1);
			REQUIRE( s.find(1) == s.end());

			s.erase(s.begin(), s.end());
			REQUIRE( s.empty());

			REQUIRE( s.erase(1) == 0);
			s.insert(1);
			REQUIRE( s.erase(1) == 1);

			s.insert(2);
			s.insert(1);
			s.insert(3);
			Iter it = s.find(3);
			s.erase(2);
			REQUIRE( *it == 3);
		}

		SECTION( "swap" ) {
			Set	s(array + 1, array + 7);
			Set	t(++s.rbegin(), --s.rend());

			s.swap(t);
			REQUIRE( s.size() == s.size());
			REQUIRE( *s.find(3) == 3);

			Set().swap(s);
			REQUIRE( s.empty());
		}
	}

	SECTION( "Lookup" ) {
		SECTION( "count and find" ) {
			Set	s;

			REQUIRE( s.count(1) == 0);
			REQUIRE( s.find(1) == s.end());
			s.insert(1);
			REQUIRE( s.count(1) == 1);
			REQUIRE( *s.find(1) == 1);
		}

		SECTION( "ranges") {
			Set						s(array, array + size);
			ft::pair<Iter, Iter>	p;

			p = s.equal_range(3);
			for (; p.first != p.second; p.first++)
				REQUIRE( *p.first == 3);

			p.first = s.lower_bound(5);
			p.second = s.upper_bound(12);
			uint i = 0;
			for (; p.first != p.second; p.first++)
				i++;
			REQUIRE( i == 4);
		}
	}

	SECTION( "observers" ) {
		Set	s;

		REQUIRE( s.key_comp()(1, 2));
		REQUIRE_FALSE( s.key_comp()(1, 1));
		REQUIRE_FALSE( s.key_comp()(2, 1));
	
		REQUIRE( s.value_comp()(1, 2));
		REQUIRE_FALSE( s.value_comp()(1, 1));
		REQUIRE_FALSE( s.value_comp()(2, 1));
	}

	SECTION( "comparisons" ) {
		Set	s(array, array + size);
		Set	t(array, array + size);
	
		REQUIRE 		( t == s);
	
		REQUIRE			( s == t);
		REQUIRE_FALSE	( s != t);
		REQUIRE_FALSE	( s <  t);
		REQUIRE			( s <= t);
		REQUIRE_FALSE	( s >  t);
		REQUIRE			( s >= t);

		t.insert(16);

		REQUIRE_FALSE	( s == t);
		REQUIRE			( s != t);
		REQUIRE			( s <  t);
		REQUIRE			( s <= t);
		REQUIRE_FALSE	( s >  t);
		REQUIRE_FALSE	( s >= t);
	
		s.insert(17);

		REQUIRE_FALSE	( s == t);
		REQUIRE			( s != t);
		REQUIRE_FALSE	( s <  t);
		REQUIRE_FALSE	( s <= t);
		REQUIRE			( s >  t);
		REQUIRE			( s >= t);
	}

	SECTION( "std::swap" ) {
		Set	s(array, array + size);
		Set	t(array + 3, array + 7);

		std::swap(s, t);
		REQUIRE( *s.begin() == 4);
		REQUIRE( *t.begin() == 1);
	}
}
