#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

	// https://www.programiz.com/dsa/insertion-in-a-red-black-tree

	// Iterator order: left, middle, right, parent

#include <functional> // less
#include <algorithm> // std::find
#include "iterator.hpp"
#include "pair.hpp"
#include <iostream> // TODO: remove

#define RED 0
#define BLACK 1

namespace ft
{

template <typename T, typename Comp = std::less<T> >
class redBlackTree {
	struct Node;
	struct treeIterator;

	public:

		// Typedefs

		typedef treeIterator	iterator;
		
		redBlackTree(): _root(NULL) {};
		redBlackTree(const redBlackTree& rhs) { *this = rhs; }
		~redBlackTree() {};
		redBlackTree&	operator=(const redBlackTree& rhs) { (void)rhs; };

		// Iterators

		iterator	begin() {
			Node*	p = root();

			while (p->left)
				p = p->left;
			return iterator(p);
		}

		iterator	end() { return NULL; }
	
		// Modifiers
	
		// TODO: remove cast
		pair<iterator, bool>	insert(const T& value) {
			Node*	newNode = new Node;

			newNode->value = value;
			if (_root)
				return insertHelper(_root, newNode);
			else {
				newNode->parent = NULL;
				_root = newNode;
				return make_pair<iterator, bool>(iterator(root()), true);
			}
		}

		// Lookup

		iterator	find(const T& value) {
			return std::find(begin(), end(), value);
		}

		void	print(Node *node, int spaces = 0) const {
			if (!node)
				return ;
			spaces += 8;
			print(node->right, spaces);
			std::cout << std::endl;
			for (int i = 8; i < spaces; i++)
        		std::cout << " ";
			std::cout << node->value << std::endl;
			print(node->left, spaces);
		}
		
		// TODO: private
		Node*	root() { return _root; }

	private:
	
		// TODO: remove cast if possible
		pair<iterator, bool>	insertHelper(Node* node, Node* newNode) {
			if (Comp()(node->value, newNode->value)) {
				if (node->left)
					return insertHelper(node->left, newNode);
				else {
					node->left = newNode;
					node->left->parent = node;
				}
			}		
			if (Comp()(newNode->value, node->value)) {
				if (node->right)
					return insertHelper(node->right, newNode);
				else {
					node->right = newNode;
					node->right->parent = node;
				}
			}
			return make_pair<iterator, bool>(node, false);
		}

		struct Node {
			T		value;
			Node*	parent;
			Node*	left;
			Node*	right;
			char	color;

			Node(): left(NULL), right(NULL), color(RED) {};
		};

		struct treeIterator: public ft::iterator<bidirectional_iterator_tag, T> {
			public:
				treeIterator(Node *p = NULL): _p(p), _origin(p ? p->left : NULL) {}
				treeIterator(const treeIterator& other): _p(other._p), _origin(other._origin) {};
			
				T&  operator*() const { return _p->value; }
				T&  operator->() const { return &_p->value; }

				// Switch doesn't work with pointers, so else/if it is
				treeIterator&	operator++() {
					if (_origin == NULL) {
						_origin = _p;
						_p = _p->parent;
					}
					if (_origin == _p->left) {
						_p = _p->right;
						while (_p->left)
							_p = _p->left;
						_origin = _p->parent;
					} else if (_origin == _p->right){
						_origin = _p;
						_p = _p->parent;
					} else if (_origin == _p->parent){
						_origin = _p;
						_p = _p->right;
					} else
						throw std::logic_error("redBlackTree");
					return *this;
				}

				treeIterator	operator++(int) { treeIterator tmp (*this); ++*this; return tmp; }
			
				friend bool	operator==(const treeIterator& lhs, const treeIterator& rhs) { return lhs._p == rhs._p; }
				friend bool	operator!=(const treeIterator& lhs, const treeIterator& rhs) { return lhs._p != rhs._p; }

			private:
				Node*	_p;
				Node*	_origin;
		};
	
		// End is parent of root
		Node*	_root;
};

} // namespace ft

#endif // REDBLACKTREE_HPP
