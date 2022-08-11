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
	std::cout << value << "\n";
}

// #define PRINT
struct A {
		static int count;
	public:
		A(int n = 0): _p(new int), _index(count++) { 
				#ifdef PRINT
					std::cout << COLOR_GREEN "new: " << _index << COLOR_DEFAULT << "\n";
				#endif
			*_p = n;
		}
		~A() {
				#ifdef PRINT
					std::cout << COLOR_RED "delete: " << _index << COLOR_DEFAULT << "\n";
				#endif
			delete _p;
		}
		A(const A& other): _p(new int), _index(count++) { *this = other; }
		A&	operator=(const A& a) {
				#ifdef PRINT
					std::cout << COLOR_BLUE "copy: " << _index << COLOR_DEFAULT << "\n";
				#endif
			delete _p;
			_p = new int;
			*_p = *a._p;
			return *this;
		}
		int p() const { return *_p; }
		operator	int() const { return *_p; }
		bool	operator<(const A& rhs) { return p() < rhs.p(); }
	
	private:
		int*	_p;
		int		_index;
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
