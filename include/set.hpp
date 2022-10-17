#ifndef SET_HPP
#define SET_HPP

#include <memory.h> // std::allocator
#include <functional> // std::less

#include "rbtree.hpp"

namespace ft
{

template <	typename Key,
			typename Compare = std::less<Key>,
			typename Allocator = std::allocator<Key> >
class set: public rbtree<Key, Key, Compare, Allocator> {

		typedef rbtree<Key, Key, Compare, Allocator>	base;

	public:

		typedef Key		key_type;
		typedef	Key		value_type;
		typedef Compare	key_compare;
		typedef Compare	value_compare;

		set(): base() {}

		explicit set(const Compare& comp, const Allocator& alloc = Allocator()): base(comp, alloc) {}

		template <typename InputIt>
		set(InputIt first, InputIt last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator())
			: base(first, last, comp, alloc) {}
	
		Compare	value_comp() const { return Compare(); }
};

} // namespace ft

#endif // SET_HPP
