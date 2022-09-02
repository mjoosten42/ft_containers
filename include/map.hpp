#ifndef MAP_HPP
#define MAP_HPP

#include <memory.h> // std::allocator
#include <functional> // std::less

#include "rbtree.hpp"
#include "pair.hpp"

namespace ft
{

template <typename Pair, typename Compare>
class keyCompare {
	public:
		keyCompare(Compare comp = Compare()): mComp(comp) {}

		bool	operator()(const Pair& pair1, const Pair& pair2) const {
			return mComp(pair1.first, pair2.first);
		}

	private:
		Compare	mComp;
};

template <	typename Key, typename T,
			typename Compare = std::less<Key>,
			typename Allocator = std::allocator<ft::pair<const Key, T> > >
class map: public rbtree<ft::pair<const Key, T>, keyCompare<ft::pair<const Key, T>, Compare>, Allocator> {

		typedef keyCompare<ft::pair<const Key, T>, Compare>				mapCompare;
		typedef rbtree<ft::pair<const Key, T>, mapCompare, Allocator>	base;

	public:

		typedef	ft::pair<const Key, T>	value_type;
		typedef typename base::iterator	iterator;

		map(): base() {}

		template <typename InputIt>
		map(InputIt first, InputIt last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator())
			: base(first, last, mapCompare(comp), alloc) {}
};

} // namespace ft

#endif // MAP_HPP
