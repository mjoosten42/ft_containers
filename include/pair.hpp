#ifndef PAIR_HPP
#define PAIR_HPP

namespace ft
{

template <typename T1, typename T2>
struct pair {
	typedef T1	first_type;
	typedef T2	second_type;

	pair(): first(), second() {}
	pair(const T1& x, const T2& y): first(x), second(y) {}
	pair(const pair& p): first(p.first), second(p.second) {}

	template <typename U1, typename U2>
	pair(const pair<U1, U2> p): first(p.first), second(p.second) {}

	pair&	operator=(const pair& rhs) {
		first = rhs.first;
		second = rhs.second;
		return *this;
	}

	friend bool	operator==(const pair& lhs, const pair& rhs) { return lhs.first == rhs.first && lhs.second == rhs.second; }
	friend bool	operator!=(const pair& lhs, const pair& rhs) { return lhs.first != rhs.first || lhs.second != rhs.second; }
	friend bool	operator< (const pair& lhs, const pair& rhs) { return lhs.first <  rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second); }
	friend bool	operator<=(const pair& lhs, const pair& rhs) { return lhs <  rhs || lhs == rhs; }
	friend bool	operator> (const pair& lhs, const pair& rhs) { return rhs <  lhs; }
	friend bool	operator>=(const pair& lhs, const pair& rhs) { return rhs <= lhs; }

	T1	first;
	T2	second;
};

template <typename T1, typename T2>
pair<T1, T2>	make_pair(T1 t, T2 u) { return pair<T1, T2>(t, u); }

} // namespace ft

// TODO: remove
#include <iostream>
template <typename T1, typename T2>
std::ostream&	operator<<(std::ostream& os, const ft::pair<T1, T2> pair) {
	return os << "{ " << pair.first << ", " << pair.second << " } ";
}

#endif // PAIR_HPP
