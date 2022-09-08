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
	
		explicit	stack(const Container& cont = Container()): _cont(cont) {}
		stack(const stack& rhs) { *this = rhs; }
		~stack() {};
		stack&	operator=(const stack& rhs) { _cont = rhs._cont; return *this; }

		reference		top() { return _cont.back(); }
		const_reference	top() const { return _cont.back(); }
	
		size_type	size() const { return _cont.size(); }
		size_type	empty() const { return _cont.empty(); }
	
		void	push(const T& value) { _cont.push_back(value); }
		void	pop() { _cont.pop_back(); }
		
		friend bool	operator==(const stack& lhs, const stack& rhs) { return lhs._cont == rhs._cont; }
		friend bool	operator!=(const stack& lhs, const stack& rhs) { return lhs._cont != rhs._cont; }
		friend bool	operator< (const stack& lhs, const stack& rhs) { return lhs._cont <  rhs._cont; }
		friend bool	operator<=(const stack& lhs, const stack& rhs) { return lhs._cont <= rhs._cont; }
		friend bool	operator> (const stack& lhs, const stack& rhs) { return lhs._cont >  rhs._cont; }
		friend bool	operator>=(const stack& lhs, const stack& rhs) { return lhs._cont >= rhs._cont; }
	
	private:
		Container	_cont;
};

} // namespace ft

#endif // STACK_HPP
