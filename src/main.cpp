#include <iostream> // std::cout
#include <iomanip> // std::setw
#include <array>  // std::array

#define RED "\033[0;31m"
#define BLUE "\033[0;36m"
#define GREEN "\033[0;32m"
#define DEFAULT "\033[0m"

#ifdef STD // CREATE A REAL STL EXAMPLE
	#include <map>
	#include <stack>
	#include <vector>
	namespace ft = std;
#else
	//#include <map.hpp>
	//#include <stack.hpp>
	#include <vector.hpp>
#endif

template <typename T, typename U>
std::ostream&	operator<<(std::ostream& os, const ft::vector<T, U>& v) {
	std::cout << "size: " << std::setw(2) << v.size();
	std::cout << ", cap: " << std::setw(2) << v.capacity();
	std::cout << " { ";
	for (size_t i = 0; i < v.size(); i++) {
		if (i)
			std::cout << ", ";
		std::cout << v[i];
	}
	std::cout << " }";
	return os;
}

template <typename T>
void	print(const T& value) {
	std::cout << value << std::endl;	
}

// #define P
struct A {
		static int count;
	public:
		A(int n = 1): _p(new int), _index(count++) { 
			#ifdef P
			std::cout << GREEN "new A: " << _index << DEFAULT << std::endl;
			#endif
			*_p = n;
		}
		~A() {
			#ifdef P
			std::cout << RED "delete A: " << _index << DEFAULT << std::endl;
			#endif
			delete _p;
		}
		A(const A& a): _p(new int), _index(count++) { *_p = *a._p;
			#ifdef P
			std::cout <<  BLUE "copy A: " << _index << DEFAULT << std::endl;
			#endif
		}
		A&	operator=(const A& a) {
			delete _p;
			_p = new int;
			*_p = *a._p;
			return *this;
		}
		friend std::ostream& operator<<(std::ostream& os, const A& a) { return os << *a._p; }
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
	void	construct(pointer p, const T& value = T()) { *p = value; }
	void	destroy(pointer p) { p->~T(); }

	size_type	max_size() const { return 16; }
	bool operator==(const myAllocator&) { return true; }
	bool operator!=(const myAllocator&) { return false; }
};

int	main() {

	const A					a;
	const std::array<A, 3>	array = { 1, 2, 3 };
	const std::array<A, 3> 	b = { 4, 5, 6 };
	ft::vector<A>			v(array.begin(), array.end());

	std::cout << std::boolalpha;

	/*
	{
		print("Constructors\n");
		ft::vector<A>	v1; print(v1);
		ft::vector<A>	v3(3, a); print(v3);
		ft::vector<A>	v4(array.begin(), array.end()); print(v4);
		ft::vector<A>	v5(v); print(v5);

		print("Copy assignment\n");
		v5 = v4;
		print(v5);
	}

	{
		print("assign\n");
		v.assign(3, a);
		print(v);
		v.assign(1, a);
		print(v);
		v.assign(array.begin(), array.end());
		print(v);
	}

	{
		print("Element access\n");

		try { print(v.at(0)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v.at(-1)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v.at(3)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		try { print(v.at(SIZE_T_MAX)); }
			catch(const std::exception& e) { std::cerr << e.what() << '\n'; }
		print(v[0]);
		print(v[1]);
		print(v[2]);
		print(v.front());
		print(v.back());
		print(v.data());
	}

	{
		print("Iterators\n");
		for (ft::vector<A>::iterator it = v.begin(); it != v.end(); it++) {
			print(*it);			
		}
	
		// for (ft::vector<A>::reverse_iterator it = v.rbegin(); it != v.rend(); it++) {
		// 	print(*it);			
		// }
	}

	{
		print("Capacity\n");
		print(v);
		print(v.empty());
		v.clear();
		print(v);
		print(v.empty());
			
		print(v.max_size());
		print(v.get_allocator().max_size());

		v.reserve(10);
		print(v.capacity());
		print(v.size());
	}

	print("Modifiers\n");

	{
		print("insert\n");
		print(*v.insert(v.begin(), a));
		v.insert(v.begin() + 1, 3, a);
		v.insert(v.begin(), array.begin(), array.end());
		v.insert(v.begin() + 1, b.begin(), b.end());
		v.insert(v.begin() + 2, array.begin(), array.end());
		print(v);
		print("erase\n");
		print(*v.erase(v.begin()));
		print(*v.erase(v.begin() + 2));
		print(*v.erase(v.begin(), v.begin() + 1));
		print(*v.erase(v.begin() + 1, v.begin() + 3));
		print(*v.erase(v.begin() + 3, v.end() - 1));
		print(v);
		print(*v.erase(v.begin(), v.end()));
		print(v);
	}

	{
		print("push/pop\n");
		for (int i = 0; i < 20; i++)
			v.push_back(a);
		print(v);
		for (int i = 0; i < 10; i++)
			v.pop_back();
		print(v);
		
		print("resize\n");
		v.resize(3, a);
		print(v);
		v.resize(10, 2);
		print(v);
	}

	{
		print("swap\n");
		ft::vector<A>	u;
	
		v.assign(array.begin(), array.end());
		for (int i = 0; i < 2; i++)
			v.insert(v.begin(), v.begin(), v.end());
		u = v;
		std::reverse(u.begin(), u.end());
		print(v);
		print(u);
		for (int i = 0; i < 99; i++)
			v.swap(u);
		print(v);
		print(u);


	}
	*/

	print(v);
	v.insert(v.begin(), v.begin(), v.end());
	print(v);
	v.insert(v.begin(), v.begin(), v.end());
	print(v);

}
