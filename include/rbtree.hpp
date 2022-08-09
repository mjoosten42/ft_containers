#ifndef RBTREE_HPP
#define RBTREE_HPP

#include <functional> // less
#include <memory> // allocator
#include "tree.hpp"
#include "pair.hpp"

namespace ft
{

template <typename T>
struct rbtreeNode: public treeNode<T> {
	bool	black;
};

template <typename T, typename Comp = std::less<T>, typename Allocator = std::allocator<T> >
class rbtree: public tree<T, Comp, Allocator> {

		typedef tree<T, Comp, Allocator>	tree;
		typedef rbtreeNode<T>				Node;
			
		using	tree::begin;
		using	tree::sentinel;

	public:

		typedef typename tree::size_type	size_type;
		typedef typename tree::iterator		iterator;


		rbtree() {};
		rbtree(const rbtree& rhs) { *this = rhs; }
		~rbtree() {};
		rbtree&	operator=(const rbtree& rhs) { tree::operator=(rhs); return *this; }

		pair<iterator, bool>	insert(const T& value) {
			pair<iterator, bool> ret = tree::insert(value);
			if (!ret.second)
				return ret;
			return ret;
		}
	
		size_type	depth(Node* node = begin()) {
			size_type	n = 0;

			while (node->parent != sentinel()) {
				if (node->black)
					n++;
				node = node->parent;
			}
			return n;
		}

		size_type	height() const {
			Node*		node = sentinel();
			size_type	n = 0;

			while ((node = node->left)) {
				if (node->black)
					n++;
			}
			return n;
		}
	private:
};

} // namespace ft

#endif // RBTREE_HPP
