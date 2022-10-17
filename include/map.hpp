#ifndef MAP_HPP
#define MAP_HPP

#include <memory.h> // std::allocator
#include <functional> // std::less

#include "rbtree.hpp"
#include "pair.hpp"

namespace ft
{

template <	typename Key, typename T,
			typename Compare = std::less<Key>,
			typename Allocator = std::allocator<pair<const Key, T> > >
class map: public rbtree<Key, pair<const Key, T>, Compare, Allocator> {
		typedef rbtree<Key, pair<const Key, T>, Compare, Allocator>	base;
	public:

		typedef Key								key_type;
		typedef T								mapped_type;
		typedef	pair<const Key, T>				value_type;
		typedef Compare							key_compare;

		using typename base::size_type;
		using typename base::iterator;
		using base::find;
		using base::end;
		using base::insert;

		// Unused
		struct value_compare {
			bool operator()(const pair<const Key, T>& pair1, const pair<const Key, T>& pair2) {
				return Compare()(pair1.first, pair2.first);
			}
		};

		value_compare	value_comp() const { return value_compare(); }

		map(): base() {}

		explicit map(const Compare& comp, const Allocator& alloc = Allocator()): base(comp, alloc) {}

		template <typename InputIt>
		map(InputIt first, InputIt last,
				const Compare& comp = Compare(),
				const Allocator& alloc = Allocator())
			: base(first, last, comp, alloc) {}

		T&	at(const Key& key) {
			iterator it = find(key);

			if (it == end())
				throw std::out_of_range("map");
			return it->second;
		}

		const T&	at(const Key& key) const {
			iterator it = find(key);

			if (it == end())
				throw std::out_of_range("map");
			return it->second;
		}

		T&	operator[](const Key& key) {
			return insert(ft::make_pair(key, T())).first->second;
		}
};

} // namespace ft

#endif // MAP_HPP
