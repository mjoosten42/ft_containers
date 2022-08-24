#ifndef TEST_HPP
#define TEST_HPP

#include <iostream> // std::cout
#include <iomanip> // std::setw

#define COLOR_RED "\033[0;31m"
#define COLOR_BLUE "\033[0;36m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_DEFAULT "\033[0m"

template <typename T>
void	print(const T& value) {
	std::cout << value << "\n";
}

struct A {
		static int count;
		static int deleted;
	public:
		A(int n = 0): _p(new int), _index(count++) { 
			*_p = n;
				#ifdef PRINT
					std::cout << COLOR_GREEN "new: " << _index;
					if (_p)
						std::cout << " (" << *_p << ")";
					std::cout << COLOR_DEFAULT << "\n";
				#endif
		}
		
		~A() {
				#ifdef PRINT
					std::cout << COLOR_RED "delete: " << _index;
					if (_p)
						std::cout << " (" << *_p << ")"; 
					std::cout << COLOR_DEFAULT << "\n";
				#endif
			delete _p;
			deleted++;
		}

		A(const A& other): _p(new int), _index(count++) { *this = other; }

		A&	operator=(const A& a) {
				#ifdef PRINT
					std::cout << COLOR_BLUE "copy: " << _index;
					if (a._p)
						std::cout << " (" << *a._p << ")"; 
					std::cout << COLOR_DEFAULT << "\n";
				#endif
			delete _p;
			_p = new int;
			*_p = *a._p;
			return *this;
		}

		A&	operator=(int n) {
			*_p = n;
			return *this;
		}
		operator	int() const { return *_p; }
	
		bool	operator<(const A& rhs) { return *_p < *rhs._p; }

		friend std::istream&	operator>>(std::istream& is, A& a) {
			return is >> *a._p;
		}

		int		alive() const { return count - deleted; }
	
	public:
		int*	_p;
		int		_index;
};

namespace std {
	template <>
	void	swap(A& lhs, A& rhs) {
			#ifdef PRINT
				std::cout << COLOR_YELLOW << "swap\n" << COLOR_DEFAULT;
			#endif
		std::swap(lhs._p, rhs._p);
	}
}

int A::count = 0;
int A::deleted = 0;

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
	void	construct(pointer p, const T& value) { new (p) T(value); }
	void	destroy(pointer p) { p->~T(); }

	size_type	max_size() const { return 16; }

	bool operator==(const myAllocator&) { return true; }
	bool operator!=(const myAllocator&) { return false; }
};

#endif // TEST_HPP
