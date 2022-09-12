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
	
		explicit	stack(const Container& cont = Container()): c(cont) {}
		stack(const stack& rhs) { *this = rhs; }
		~stack() {};
		stack&	operator=(const stack& rhs) { c = rhs.c; return *this; }

		reference		top() { return c.back(); }
		const_reference	top() const { return c.back(); }
	
		size_type	size() const { return c.size(); }
		size_type	empty() const { return c.empty(); }
	
		void	push(const T& value) { c.push_back(value); }
		void	pop() { c.pop_back(); }

		friend bool	operator==(const stack& lhs, const stack& rhs) { return lhs.c == rhs.c; }
		friend bool	operator!=(const stack& lhs, const stack& rhs) { return lhs.c != rhs.c; }
		friend bool	operator< (const stack& lhs, const stack& rhs) { return lhs.c <  rhs.c; }
		friend bool	operator<=(const stack& lhs, const stack& rhs) { return lhs.c <= rhs.c; }
		friend bool	operator> (const stack& lhs, const stack& rhs) { return lhs.c >  rhs.c; }
		friend bool	operator>=(const stack& lhs, const stack& rhs) { return lhs.c >= rhs.c; }
	
	protected:
		Container	c;
};
		
} // namespace ft

#endif // STACK_HPP
