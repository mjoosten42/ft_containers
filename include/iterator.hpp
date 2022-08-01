#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cstddef>

namespace ft
{

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag:		public input_iterator_tag {};
struct bidirectional_iterator_tag:	public forward_iterator_tag {};
struct random_access_iterator_tag:	public bidirectional_iterator_tag {};
struct contigous_iterator_tag:		public random_access_iterator_tag {};

template<class Iterator>
struct iterator_traits
{
	typedef typename Iterator::difference_type		difference_type;
	typedef typename Iterator::value_type			value_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
	typedef typename Iterator::iterator_category	iterator_category;
};

template<class T>
struct iterator_traits<T*>
{
	typedef std::ptrdiff_t				difference_type;
	typedef T							value_type;
	typedef T*							pointer;
	typedef T&							reference;
	typedef random_access_iterator_tag	iterator_category;
};

template <class Category, class T, class Distance = std::size_t,
		class Pointer = T*, class Reference = T&>
struct iterator
{
	typedef Category	iterator_category;
	typedef T			value_type;
	typedef Distance	difference_type;
	typedef Pointer		pointer;
	typedef Reference	reference;
};

template <typename T>
class reverseIterator: public iterator<contigous_iterator_tag, T> {
	public:
		reverseIterator(T* p): _p(p) {}
		reverseIterator(const reverseIterator& other): _p(other._p) {}

		T&  operator*() const { return *(_p - 1); }
		T&  operator->() const { return _p - 1; }

		reverseIterator&	operator++() { return *(_p--); }
		reverseIterator&	operator--() { return *(_p++); }
		reverseIterator		operator++(int) { reverseIterator tmp = *this; _p--; return tmp; }
		reverseIterator		operator--(int) { reverseIterator tmp = *this; _p++; return tmp; }

		friend bool	operator==(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs._p == rhs._p; }
		friend bool	operator!=(const reverseIterator& lhs, const reverseIterator& rhs) { return lhs._p != rhs._p; }

	private:
		T*  _p;
};

} // namespace ft

#endif // ITERATOR_HPP
