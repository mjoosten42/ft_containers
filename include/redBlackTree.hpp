#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

	// https://www.programiz.com/dsa/insertion-in-a-red-black-tree
	// https://visualgo.net/en/bst
	// https://adtinfo.org/libavl.html/Deleting-from-a-BST.html (erase)

#include <functional> // std::less
#include <memory> // std::allocator
#include "iterator.hpp"
#include "pair.hpp"
#include <iostream> // TODO: remove
#include <iomanip> // TODO: remove

#define RED	0
#define BLACK 1

#define LEFT_OR_RIGHT_CHILD(node) node->parent->left == node ? &node->parent->left : &node->parent->right

namespace ft
{

template <typename T, typename Comp = std::less<T>, typename Allocator = std::allocator<T> >
class redBlackTree {

	struct Node {
		T		value;
		Node*	parent;
		Node*	left;
		Node*	right;
		char	color;

		Node(const T& value = T()): value(value), parent(NULL), left(NULL), right(NULL), color(RED) {};

		// TODO: remove
		friend std::ostream&	operator<<(std::ostream& os, const Node& node) {
			os << "Node: { " << node.value;
			os << ", " << node.parent;
			os << ", " << node.left;
			os << ", " << node.right;
			return os << " }";
		}
	};

	struct treeIterator: public ft::iterator<bidirectional_iterator_tag, T> {
		public:
			treeIterator() {};
			treeIterator(Node *p): _p(p) {}
			treeIterator(const treeIterator& other): _p(other._p) {};

			treeIterator&	operator=(const treeIterator& other) { _p = other._p; return *this; }
		
			T&  operator*() const { return _p->value; }
			T&  operator->() const { return &_p->value; }

			treeIterator&	operator++() {
				Node*	q = _p;
			
				if (_p->right) {
					_p = _p->right;
					while (_p->left)
						_p = _p->left;
				}
				else {
					_p = _p->parent;
					while (q == _p->right) {
						q = _p;
						_p = _p->parent;
					}
				}
				return *this;
			}

			treeIterator&	operator--() {
				Node*	q = _p;
			
				if (_p->left) {
					_p = _p->left;
					while (_p->right)
						_p = _p->right;
				}
				else {
					_p = _p->parent;
					while (q == _p->left) {
						q = _p;
						_p = _p->parent;
					}
				}
				return *this;
			}

			treeIterator	operator++(int) { treeIterator tmp (*this); ++*this; return tmp; }
			treeIterator	operator--(int) { treeIterator tmp (*this); --*this; return tmp; }

			friend bool	operator==(const treeIterator& lhs, const treeIterator& rhs) { return lhs._p == rhs._p; }
			friend bool	operator!=(const treeIterator& lhs, const treeIterator& rhs) { return lhs._p != rhs._p; }

			operator Node*() const { return _p; }

		private:
			Node*	_p;
	};

	public:

		// Typedefs

		typedef std::size_t						size_type;
		typedef treeIterator					iterator;
		typedef reverseIterator<treeIterator>	reverse_iterator;
	
		typedef typename Allocator::template rebind<Node>::other	NodeAllocator;
		
		redBlackTree(): _sentinel(newNode(T())) {};
		redBlackTree(const redBlackTree& rhs): _sentinel(newNode(T())) { *this = rhs; }
		~redBlackTree() {}; // TODO
		redBlackTree&	operator=(const redBlackTree& rhs) { (void)rhs; }; // TODO

		// Element access

		T&	operator[](const T& value) {
			iterator it = find(value);
		
			if (it != end())
				return *it;
			return *insert(value).first;
		}

		// Iterators

		iterator	begin() const {
			Node*	p = sentinel();

			while (p->left)
				p = p->left;
			return p;
		}

		iterator	end() const { return sentinel(); }

		reverse_iterator	rbegin() const { return end(); }

		reverse_iterator	rend() const { return begin(); }
	
	
		// Capacity

		bool		empty() const { return root() == NULL; }
		size_type	size() const { return sizeHelper(root()); }
	
		// Modifiers

		void	swap(redBlackTree& other) {
			std::swap(_alloc);
			std::swap(sentinel(), other.sentinel());
		}

		pair<iterator, bool>	insert(const T& value) {
			Node*	p = root();
			Node*	q = sentinel();

			while (p) {
				q = p;
				if (Comp()(value, p->value))
					p = p->left;
				else if (Comp()(p->value, value))
					p = p->right;
				else
					return make_pair<iterator, bool>(p, false);
			}
			Node*	tmp = newNode(value);
			tmp->parent = q;
			if (q == sentinel())
				_sentinel->left = tmp;
			else if (Comp()(value, q->value))
				q->left = tmp;
			else
				q->right = tmp;
			return make_pair<iterator, bool>(tmp, true);
		}

		void	erase(iterator pos) {
			Node*	node = pos;

			if (!node->left && !node->right)
				eraseLeaf(node);
			else if (node->left && node->right)
				(void)node;
			else
				eraseOneChild(node);
		}
	
		size_type	erase(const T& value) {
			iterator it = find(value);

			if (it == end())
				return 0;
			erase(it);
			return 1;
		}

		// Lookup

		size_type	count(const T& value) const {
			return find(value) != end() ? 1 : 0;
		}
	
		iterator	find(const T& value) const {
			Node*	p = root();
	
			while (p) {
				if (Comp()(value, p->value))
					p = p->left;
				else if (Comp()(p->value, value))
					p = p->right;
				else
					break ;
			}
			return p;
		}
		
		// first element >= value
		iterator	lower_bound(const T& value) {
			iterator it = begin();

			while (Comp()(*it, value))
				it++;
			return it;
		}
	
		// first element > value
		iterator	upper_bound(const T& value) {
			iterator it = lower_bound(value);

			if (!Comp()(value, *it))
				it++;
			return it;
		}
	
		// TODO: remove
		void	print(Node *node, int spaces = 0) const {
			if (!node)
				return ;
			spaces += 8;
			print(node->right, spaces);
			std::cout << std::endl;
			for (int i = 8; i < spaces; i++)
        		std::cout << " ";
			std::cout << std::setw(2) << node->value << std::endl;
			print(node->left, spaces);
		}
		
		// TODO: private
		Node*	root() const { return _sentinel->left; }

	private:
	
		Node*	sentinel() const { return _sentinel; }

		Node*	newNode(const T& value) {
			Node*	tmp = _alloc.allocate(1);

			_alloc.construct(tmp, value);
			return tmp;
		}
	
		size_type	sizeHelper(Node* node) const {
			if (!node)
				return 0;
			return sizeHelper(node->left) + 1 + sizeHelper(node->right);
		}
	
		void	eraseLeaf(Node* node) {
			if (node->parent)
				*(LEFT_OR_RIGHT_CHILD(node)) = NULL;
			else
				_sentinel->left = NULL;
			deleteNode(node);
		}

		// TODO: rewrite
		void	eraseOneChild(Node* node) {
			if (node->left && !node->right) {
				node->left->parent = node->parent;
				if (node->parent) {
					if (node->parent->left == node)
						node->parent->left = node->left;
					else
						node->parent->right = node->left;
				}
				else
					_sentinel->left = node->left;
			}
			else {
				node->right->parent = node->parent;
				if (node->parent) {
					if (node->parent->right == node)
						node->parent->right = node->right;
					else
						node->parent->left = node->right;
				}
				else
					_sentinel->left = node->right;
			}
		}

		void	deleteNode(Node* node) {
			_alloc.destroy(node);
			_alloc.deallocate(node, 1);
		}
	
		NodeAllocator	_alloc;
		Node*			_sentinel;
};

} // namespace ft

#endif // REDBLACKTREE_HPP
