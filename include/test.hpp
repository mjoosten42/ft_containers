#ifndef TEST_HPP
#define TEST_HPP

#include <iostream> // std::cout
#include <iomanip> // std::setw

#define COLOR_RED "\033[0;31m"
#define COLOR_BLUE "\033[0;36m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_DEFAULT "\033[0m"

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
			std::cout << COLOR_GREEN "new A: " << _index << COLOR_DEFAULT << std::endl;
			#endif
			*_p = n;
		}
		~A() {
			#ifdef PRINT
			std::cout << COLOR_RED "delete A: " << _index << COLOR_DEFAULT << std::endl;
			#endif
			delete _p;
		}
		A(const A& a): _p(new int), _index(count++) { *_p = *a._p;
			#ifdef PRINT
			std::cout <<  COLOR_BLUE "copy A: " << _index << COLOR_DEFAULT << std::endl;
			#endif
		}
		A&	operator=(const A& a) {
			delete _p;
			_p = new int;
			*_p = *a._p;
			return *this;
		}
		int p() const { return *_p; }
		operator	int() const { return *_p; }
		friend std::ostream& operator<<(std::ostream& os, const A& a) { return os << *a._p; }
		friend bool	operator==(const A& lhs, const A& rhs) { return lhs.p() == rhs.p(); }
		friend bool	operator!=(const A& lhs, const A& rhs) { return lhs.p() != rhs.p(); }
		friend bool	operator< (const A& lhs, const A& rhs) { return lhs.p() <  rhs.p(); }
		friend bool	operator<=(const A& lhs, const A& rhs) { return lhs.p() <= rhs.p(); }
		friend bool	operator> (const A& lhs, const A& rhs) { return lhs.p() >  rhs.p(); }
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
