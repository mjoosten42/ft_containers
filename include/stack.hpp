#ifndef STACK_HPP
#define STACK_HPP

#include "vector.hpp"

namespace ft
{

template <typename T, typename Container = vector<T> >
class stack {
	public:
		typedef			 Container					container_type;
		typedef typename Container::value_type		value_type;
		typedef typename Container::size_type		size_type;
		typedef typename Container::reference		reference;
		typedef typename Container::const_reference	const_reference;
	
		explicit	stack(const Container& cont = Container()): _v(cont.begin(), cont.end()) {}
		stack(const stack& rhs) { *this = rhs; }
		~stack() {};
		stack&	operator=(const stack& rhs) { _v = rhs._v; }

		reference	top() const { return _v.back(); }
		size_type	empty() const { return _v.empty(); }
		size_type	size() const { return _v.size(); }
	
		void	push(const T& value) { _v.push_back(value); }
		void	pop(const T& value) { _v.pop_back(value); }
		
		friend bool	operator==(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs.v == rhs._v; }
		friend bool	operator!=(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs._v != rhs._v; }
		friend bool	operator<(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs._v < rhs._v; }
		friend bool	operator<=(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs._v <= rhs._v; }
		friend bool	operator>(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs._v > rhs._v; }
		friend bool	operator>=(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs._v >= rhs._v; }
	
	private:
		Container	_v;
};

} // namespace ft

#endif // STACK_HPP
