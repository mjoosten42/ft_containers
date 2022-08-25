#ifndef MYINT_HPP
#define MYINT_HPP

#include <sstream>

struct myInt {
		static int count;
		static int deleted;
	public:
		myInt(int n = 0): _p(new int), _index(count++) { *_p = n; }
		myInt(const myInt& other): _p(new int), _index(count++) { *this = other; }
		
		~myInt() {
			delete _p;
			deleted++;
		}

		myInt&	operator=(const myInt& rhs) {
			delete _p;
			_p = new int;
			*_p = *rhs._p;
			return *this;
		}

		myInt&	operator=(int n) {
			*_p = n;
			return *this;
		}

		operator	int() const { return *_p; }
	
		friend bool	operator<(const myInt& lhs, const myInt& rhs) { return *lhs._p < *rhs._p; }

		int		alive() const { return count - deleted; }
	
	public:
		int*	_p;
		int		_index;
};

std::istream&	operator>>(std::istream& is, myInt& rhs) {
	return is >> *rhs._p;
}

namespace std {
	template <>
	void	swap(myInt& lhs, myInt& rhs) {
		std::swap(lhs._p, rhs._p);
	}
}

int myInt::count = 0;
int myInt::deleted = 0;

#endif
