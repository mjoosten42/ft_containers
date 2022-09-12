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

		Compare	mComp;
};

template <	typename Key, typename T,
			typename Compare = std::less<Key>,
			typename Allocator = std::allocator<pair<const Key, T> > >
class map: public rbtree<pair<const Key, T>, keyCompare<pair<const Key, T>, Compare>, Allocator> {
		typedef rbtree<pair<const Key, T>, keyCompare<pair<const Key, T>, Compare>, Allocator>	base;
	public:

		typedef Key					key_type;
		typedef T					mapped_type;
		typedef	pair<const Key, T>	value_type;
		typedef Compare				key_compare;

		using value_compare = keyCompare<value_type, Compare>;

		using typename base::size_type;
		using typename base::iterator;
		using base::erase;

		map(): base() {}

		explicit map(const Compare& comp, const Allocator& alloc = Allocator()): base(comp, alloc) {}

		template <typename InputIt>
		map(InputIt first, InputIt last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator())
			: base(first, last, value_compare(comp), alloc) {}

		T&	at(const Key& key) {
			iterator it = base::find(ft::make_pair(key, T()));

			if (it == base::end())
				throw std::out_of_range("map");
			return it->second;
		}

		const T&	at(const Key& key) const {
			iterator it = base::find(ft::make_pair(key, T()));

			if (it == base::end())
				throw std::out_of_range("map");
			return it->second;
		}

		T&	operator[](const Key& key) {
			return base::insert(ft::make_pair(key, T())).first->second;
		}

		size_type	erase(const Key& key) {
			return base::erase(ft::make_pair(key, T()));
		}

		iterator	find(const Key& key) const {
			return base::find(ft::make_pair(key, T()));
		}

		size_type	count(const Key& key) const {
			return base::count(ft::make_pair(key, T()));
		}

		pair<iterator, iterator>	equal_range(const Key& key) const {
			return base::equal_range(ft::make_pair(key, T()));
		}

		iterator	lower_bound(const Key& key) const {
			return base::lower_bound(ft::make_pair(key, T()));
		}

		iterator	upper_bound(const Key& key) const {
			return base::upper_bound(ft::make_pair(key, T()));
		}

		Compare	key_comp() const {
			return Compare();
		}
};

} // namespace ft

#endif // MAP_HPP
