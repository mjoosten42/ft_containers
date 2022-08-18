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
	
		explicit	stack(const Container& cont = Container()): _cont(cont.begin(), cont.end()) {}
		stack(const stack& rhs) { *this = rhs; }
		~stack() {};
		stack&	operator=(const stack& rhs) { _cont = rhs._cont; }

		reference	top() const { return _cont.back(); }
		size_type	size() const { return _cont.size(); }
		size_type	empty() const { return _cont.empty(); }
	
		void	push(const T& value) { _cont.push_back(value); }
		void	pop(const T& value) { _cont.pop_back(value); }
		
		bool	operator==(const stack& rhs) { return _cont == rhs._cont; }
		bool	operator!=(const stack& rhs) { return _cont != rhs._cont; }
		bool	operator< (const stack& rhs) { return _cont <  rhs._cont; }
		bool	operator<=(const stack& rhs) { return _cont <= rhs._cont; }
		bool	operator> (const stack& rhs) { return _cont >  rhs._cont; }
		bool	operator>=(const stack& rhs) { return _cont >= rhs._cont; }
	
	private:
		Container	_cont;
};

} // namespace ft

namespace std {
	template <typename T, typename Container>
	void	swap(ft::stack<T, Container>& lhs, ft::stack<T, Container>& rhs) { lhs.swap(rhs); }
}

#endif // STACK_HPP
