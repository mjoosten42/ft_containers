#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <functional> // less
#include <memory> // allocator
#include "tree.hpp"
#include "pair.hpp"

namespace ft
{

template <typename T, typename Comp = std::less<T>, typename Allocator = std::allocator<T> >
class rbtree: public tree<T, Comp, Allocator> {

		typedef tree<T, Comp, Allocator>	tree;
		typedef typename tree::Node			Node;

	public:

		typedef typename tree::size_type	size_type;
		typedef typename tree::iterator		iterator;

		rbtree() {};
		rbtree(const rbtree& rhs) { *this = rhs; }
		~rbtree() {};
		rbtree&	operator=(const rbtree& rhs) { tree::operator=(rhs); return *this; } // TODO

		pair<iterator, bool>	insert(const T& value) {
			pair<iterator, bool> ret = tree::insert(value);
			if (!ret.second)
				return ret;
			return ret;
		}
	
		size_type	depth(Node* node = tree::sentinel()) {
			return 0; (void)node;
		}

		size_type	height() const {
			Node*		p = tree::sentinel();
			size_type	n = 0;

			while ((p = p->left))
				n++;
			return n;
		}
	private:
};

} // namespace ft

#endif // RBTREE_HPP
