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

TEMPLATE_TEST_CASE( "map", "[map]", myInt ) {
	typedef ft::map<TestType, std::string>	Map;

	const ft::pair<TestType, std::string>	array[] = {
		{ 8,  "h" },
		{ 3,  "c" },
		{ 1,  "a" },
		{ 6,  "f" },
		{ 4,  "d" },
		{ 7,  "g" },
		{ 10, "j" },
		{ 14, "n" },
		{ 13, "m" }
	};
	const uint	size = sizeof(array) / sizeof(array[0]);

	SECTION( "constructors") {
		SECTION( "default") {
			Map	m;

			REQUIRE( m.empty());
		}

		SECTION( "custom Compare") {

		}
	
		SECTION( "InputIt") {
			Map	m(array, array + size);

			REQUIRE_FALSE( m.empty());
			REQUIRE( m.size() == size);
		}

		SECTION( "copy constructor") {
			Map	m(array, array + size);
			Map	n(m);

			REQUIRE( n.size() == m.size());
		}
	}

	SECTION( "copy assignment" ) {
		Map m(array, array + size);
		Map n(array + 2, array + 5);

		std::cout << __LINE__ << "\n";
		n = m;
		std::cout << __LINE__ << "\n";
	
		REQUIRE( n.size() == m.size());
	}
}
