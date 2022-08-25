#ifndef MAP_HPP
#define MAP_HPP

#include <memory.h> // std::allocator
#include <functional> // std::less

#include "rbtree.hpp"
#include "pair.hpp"

namespace ft
{

template <	typename Key, typename T, typename Compare = std::less<Key>,
			typename Allocator = std::allocator<std::pair<const Key, T> > >
class map: public rbtree<ft::pair<const Key, T>, Compare, Allocator> {

		typedef rbtree<ft::pair<const Key, T>, Compare, Allocator>	base;

	public:
	
		using base::empty;

		map(): base() {};

		template <typename InputIt>
		map(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: base(first, last, comp, alloc) {};
};

} // namespace ft

#endif // MAP_HPP
