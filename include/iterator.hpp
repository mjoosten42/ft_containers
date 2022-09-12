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

		reverseIterator(): current() {}
	
		explicit	reverseIterator(Iterator it): current(it) {}

		template <typename U>
		reverseIterator(const reverseIterator<U>& other): current(other.base()) {}

		template <typename U>
		reverseIterator&	operator=(const reverseIterator<U>& rhs) {
			current = rhs.base();
			return *this;
		}

		Iterator	base() const { return current; }

		reference	operator*() const { Iterator it = current; return *--it; }
		pointer		operator->() const { return &operator*(); }

		reference	operator[](difference_type n) const { Iterator it = current; return *(it - n - 1); }
	
		reverseIterator&	operator++() { --current; return *this; }
		reverseIterator&	operator--() { ++current; return *this; }
	
		reverseIterator		operator++(int) { reverseIterator tmp(*this); ++(*this); return tmp; }
		reverseIterator		operator--(int) { reverseIterator tmp(*this); --(*this); return tmp; }

		reverseIterator		operator+ (difference_type n) const { return reverseIterator(current + n); }
		reverseIterator		operator- (difference_type n) const { return reverseIterator(current - n); }
	
		reverseIterator&	operator+=(difference_type n) const { current += n; return *this; }
		reverseIterator&	operator-=(difference_type n) const { current -= n; return *this; }
	
	protected:
		Iterator	current;
};

template <typename Iterator>
bool	operator==(const reverseIterator<Iterator>& lhs, const reverseIterator<Iterator>& rhs) {
	return lhs.base() == rhs.base();
}

template <typename Iterator>
bool	operator!=(const reverseIterator<Iterator>& lhs, const reverseIterator<Iterator>& rhs) {
	return lhs.base() != rhs.base();
}

template <typename Iterator>
bool	operator< (const reverseIterator<Iterator>& lhs, const reverseIterator<Iterator>& rhs) {
	return lhs.base() >  rhs.base();
}

template <typename Iterator>
bool	operator<=(const reverseIterator<Iterator>& lhs, const reverseIterator<Iterator>& rhs) {
	return lhs.base() >= rhs.base();
}

template <typename Iterator>
bool	operator> (const reverseIterator<Iterator>& lhs, const reverseIterator<Iterator>& rhs) {
	return lhs.base() <  rhs.base();
}

template <typename Iterator>
bool	operator>=(const reverseIterator<Iterator>& lhs, const reverseIterator<Iterator>& rhs) {
	return lhs.base() <= rhs.base();
}

} // namespace ft

#endif // ITERATOR_HPP
