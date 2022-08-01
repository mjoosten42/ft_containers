#ifndef TEST_HPP
#define TEST_HPP

#include <iostream> // std::cout
#include <iomanip> // std::setw

#define RED "\033[0;31m"
#define BLUE "\033[0;36m"
#define GREEN "\033[0;32m"
#define DEFAULT "\033[0m"

#ifdef STD 
	#include <vector>
	#include <stack>
	#include <map>
	namespace ft = std;
#else
	#include "vector.hpp"
	#include "stack.hpp"
	// #include "map.hpp"
#endif

template <typename T>
void	print(const T& value) {
	std::cout << value << std::endl;	
}

// #define PRINT
struct A {
		static int count;
	public:
		A(int n = 1): _p(new int), _index(count++) { 
			#ifdef PRINT
			std::cout << GREEN "new A: " << _index << DEFAULT << std::endl;
			#endif
			*_p = n;
		}
		~A() {
			#ifdef PRINT
			std::cout << RED "delete A: " << _index << DEFAULT << std::endl;
			#endif
			delete _p;
		}
		A(const A& a): _p(new int), _index(count++) { *_p = *a._p;
			#ifdef PRINT
			std::cout <<  BLUE "copy A: " << _index << DEFAULT << std::endl;
			#endif
		}
		A&	operator=(const A& a) {
			delete _p;
			_p = new int;
			*_p = *a._p;
			return *this;
		}
		int p() const { return *_p; }
		friend std::ostream& operator<<(std::ostream& os, const A& a) { return os << *a._p; }
		friend bool	operator==(const A& lhs, const A& rhs) { return lhs.p() == rhs.p(); }
		friend bool	operator!=(const A& lhs, const A& rhs) { return lhs.p() != rhs.p(); }
		friend bool	operator<(const A& lhs, const A& rhs) { return lhs.p() < rhs.p(); }
		friend bool	operator<=(const A& lhs, const A& rhs) { return lhs.p() <= rhs.p(); }
		friend bool	operator>(const A& lhs, const A& rhs) { return lhs.p() > rhs.p(); }
		friend bool	operator>=(const A& lhs, const A& rhs) { return lhs.p() >= rhs.p(); }
	
	private:
		int	*_p;
		int	_index;
};


int A::count = 0;

template <typename T>
class myAllocator {
	typedef T				value_type;
	typedef T*				pointer;
	typedef const T*		const_pointer;
	typedef T&				reference;
	typedef const T&		const_reference;
	typedef std::size_t		size_type;
	typedef std::ptrdiff_t	difference_type;

	pointer	allocate(size_type n) { return malloc(n * sizeof(T)); }
	void	deallocate(pointer p, size_type n) { free(p); (void)n; }
	void	construct(pointer p, const T& value = T()) { new (p) T(value); }
	void	destroy(pointer p) { p->~T(); }

	size_type	max_size() const { return 16; }

	bool operator==(const myAllocator&) { return true; }
	bool operator!=(const myAllocator&) { return false; }
};

#endif // TEST_HPP
