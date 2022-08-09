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
	pair(const pair& p) { *this = p; }

	pair&	operator=(const pair& rhs) {
		first = rhs.first;
		second = rhs.second;
		return *this;
	}

	bool	operator==(const pair& rhs) { return first == rhs.first && second == rhs.second; }
	bool	operator!=(const pair& rhs) { return first != rhs.first || second != rhs.second; }
	bool	operator< (const pair& rhs) { return first <  rhs.first || (first == rhs.first && second < rhs.second); }
	bool	operator<=(const pair& rhs) { return *this <  rhs || *this == rhs; }
	bool	operator> (const pair& rhs) { return rhs <= *this; }
	bool	operator>=(const pair& rhs) { return rhs <  *this || *this == rhs; }

	T1	first;
	T2	second;
};

template <typename T1, typename T2>
pair<T1, T2>	make_pair(T1 t, T2 u) { return pair<T1, T2>(t, u); }

} // namespace ft

#endif // PAIR_HPP
