#ifndef MAP_HPP
#define MAP_HPP

#include <memory.h> // std::allocator
#include <functional> // std::less
#include "pair.hpp"

namespace ft
{

template <	typename Key, typename T, typename Compare = std::less<Key>,
			typename Allocator = std::allocator<std::pair<const Key, T> > >
class map {
	public:

		// Typedefs

		typedef Key								key_type;
		typedef pair<const Key, T> 				mapped_type;
		typedef std::size_t						value_type;
		typedef std::ptrdiff_t 					difference_type;
		typedef Compare							key_compare;
		typedef Allocator 						allocator_type;
		typedef value_type&						reference;
		typedef const value_type& 				const_reference;
		typedef typename Allocator::pointer		pointer;
		typedef typename Allocator::const_pointer	const_pointer;
		// typedef T*	iterator;
		// typedef const T* const_iterator;
		// typedef T*	reverse_iterator;
		// typedef const T* const_reverse_iterator;

	private:
		Allocator	_allocator;
};

} // namespace ft

#endif // MAP_HPP
