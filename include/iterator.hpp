#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <cstddef>
#include <iterator> // tags

namespace ft
{

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
	typedef std::ptrdiff_t					difference_type;
	typedef T								value_type;
	typedef T*								pointer;
	typedef T&								reference;
	typedef std::random_access_iterator_tag	iterator_category;
};

template <class Iterator>
class reverseIterator {
	public:
		typedef Iterator												iterator_type;
		typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename iterator_traits<Iterator>::value_type			value_type;
		typedef typename iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename iterator_traits<Iterator>::pointer				pointer;
		typedef typename iterator_traits<Iterator>::reference			reference;

		reverseIterator(): _it() {};
		reverseIterator(const Iterator& it): _it(it) {};
		reverseIterator(const reverseIterator& other): _it(other._it) {};

		reference	operator*() const { Iterator it(_it); return *--it; }
		reference	operator->() const { return &operator*(); }
	
		reverseIterator&	operator++() { --_it; return *this; }
		reverseIterator&	operator--() { ++_it; return *this; }
		reverseIterator		operator++(int) { reverseIterator tmp(*this); ++(*this); return tmp; }
		reverseIterator		operator--(int) { reverseIterator tmp(*this); --(*this); return tmp; }

		bool	operator==(const reverseIterator& rhs) { return _it == rhs._it; }
		bool	operator!=(const reverseIterator& rhs) { return _it != rhs._it; }
	
	private:
		Iterator	_it;
};

} // namespace ft

#endif // ITERATOR_HPP
