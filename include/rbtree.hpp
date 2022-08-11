#ifndef RBTREE_HPP
#define RBTREE_HPP

	// https://www.programiz.com/dsa/insertion-in-a-red-black-tree
	// https://adtinfo.org/libavl.html/RB-Balancing-Rule.html
	// https://www.cs.usfca.edu/~galles/visualization/RedBlack.html

#include <functional> // std::less
#include <memory> // std::allocator
#include <iterator> // bidirectional_iterator_tag
#include "iterator.hpp" // reverseIterator
#include "pair.hpp"
#include <iostream> // TODO: remove
#include <iomanip> // TODO: remove

namespace ft
{

template <typename T>
struct Node {
	T		value;
	Node*	parent;
	Node*	left;
	Node*	right;
	bool	black;

	Node(const T& value): value(value), left(NULL), right(NULL), black(false) {};

	// TODO: remove
	friend std::ostream&	operator<<(std::ostream& os, const Node& node) {
		os << "Node: { " << node.value;
		os << ", " << node.parent;
		os << ", " << node.left;
		os << ", " << node.right;
		os << (node.black ? ", black" : ", red");
		os << " }";
		return os;
	}
};

template <typename T>
struct rbtreeIterator {
		typedef Node<T>	Node;
	public:
		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::ptrdiff_t					difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	
		rbtreeIterator() {};
		rbtreeIterator(Node *p): _p(p) {}
		rbtreeIterator(const rbtreeIterator& other): _p(other._p) {};

		rbtreeIterator&	operator=(const rbtreeIterator& other) { _p = other._p; return *this; }
	
		T&  operator*() const { return _p->value; }
		T&  operator->() const { return &_p->value; }

		rbtreeIterator&	operator++() {
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

		rbtreeIterator&	operator--() {
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

		rbtreeIterator	operator++(int) { rbtreeIterator tmp (*this); ++*this; return tmp; }
		rbtreeIterator	operator--(int) { rbtreeIterator tmp (*this); --*this; return tmp; }

		bool	operator==(const rbtreeIterator& rhs) { return _p == rhs._p; }
		bool	operator!=(const rbtreeIterator& rhs) { return _p != rhs._p; }

		operator Node*() const { return _p; }

	private:
		Node*	_p;
};

template <typename T, typename Comp = std::less<T>, typename Allocator = std::allocator<T> >
class rbtree {
	protected:

		typedef Node<T>	Node;
		typedef typename Allocator::template rebind<Node>::other	NodeAllocator;

	public:

		// Typedefs

		typedef std::size_t					size_type;
		typedef rbtreeIterator<T>			iterator;
		typedef reverseIterator<iterator>	reverse_iterator;
		
		rbtree(): _alloc(), _comp(), _sentinel(newNode(T(), NULL)) { _sentinel->black = true; }
		rbtree(const rbtree& rhs): _sentinel(NULL) { *this = rhs; }

		~rbtree() {
			clear();
			deleteNode(sentinel());
		};

		rbtree&	operator=(const rbtree& rhs) {
			if (sentinel())
				destroySubtree(sentinel());
			_alloc = rhs.get_allocator();
			_comp = rhs.value_comp();
			_sentinel = newNode(T(), NULL);
			_sentinel->black = true;
			insert(rhs.begin(), rhs.end());
			return *this;
		}

		Allocator	get_allocator() const { return _alloc; }

		// Element access

		T&	at(const T& value) {
			iterator it = find(value);

			if (it == end())
				throw std::out_of_range("tree");
			return *it;
		}
	
		T&	operator[](const T& value) {
			iterator it = find(value);
		
			if (it == end())
				return *insert(value).first;
			return *it;
		}

		// Iterators

		iterator	begin() const {
			Node*	p = sentinel();

			while (p->left)
				p = p->left;
			return p;
		}

		iterator			end() const { return sentinel(); }
		reverse_iterator	rbegin() const { return end(); }
		reverse_iterator	rend() const { return begin(); }
	
	
		// Capacity

		bool		empty() const { return size() == 0; }
		size_type	size() const { return std::distance(begin(), end()); }
		size_type	max_size() const { return _alloc.max_size(); }
	
		// Modifiers

		void	clear() {
			destroySubtree(root());
			sentinel()->left = NULL;
		}

		void	swap(rbtree& other) {
			std::swap(_alloc);
			std::swap(_comp);
			std::swap(sentinel(), other.sentinel());
		}

		pair<iterator, bool>	insert(const T& value) {
			Node*	tmp;
			Node*	p = root();
			Node*	q = sentinel();

			while (p) {
				q = p;
				if (_comp(value, p->value))
					p = p->left;
				else if (_comp(p->value, value))
					p = p->right;
				else
					return make_pair<iterator, bool>(p, false);
			}
			tmp = newNode(value, q);
			if (q == sentinel())
				sentinel()->left = tmp;
			else
				(_comp(value, q->value) ? q->left : q->right) = tmp;
			rebalance(tmp);
			return make_pair<iterator, bool>(tmp, true);
		}
	
		template <typename InputIt>
		void	insert(InputIt first, InputIt last) {
			for (; first != last; first++)
				insert(*first);
		}

		void	erase(iterator pos) {
			Node*	node = pos;

			switch (amountOfChildren(node)) {
			case 0:
				parentsChild(node) = NULL;
				break;
			case 1:
				leftOrRightChild(node)->parent = node->parent;
				parentsChild(node) = leftOrRightChild(node);
				break;
			case 2:
				Node*	successor = node->right;

				while (successor->left)
					successor = successor->left;
				std::swap(node->value, successor->value); // invalidates wrong iterator
				return erase(successor);
			}
			deleteNode(node);
		}
	
		void	erase(iterator first, iterator last) {
			iterator it;

			while (first != last) {
				first++;
				it = first;
				erase(--it);
			}
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
				if (_comp(value, p->value))
					p = p->left;
				else if (_comp(p->value, value))
					p = p->right;
				else
					return p;
			}
			return end();
		}
		
		pair<iterator, iterator>	equal_range(const T& value) {
			return make_pair(lower_bound(value), upper_bound(value));
		}

		// first element >= value
		iterator	lower_bound(const T& value) {
			iterator it = begin();

			while (_comp(*it, value))
				it++;
			return it;
		}
	
		// first element > value
		iterator	upper_bound(const T& value) {
			iterator it = lower_bound(value);

			if (!_comp(value, *it))
				it++;
			return it;
		}
	
		// Observers

		Comp	value_comp() const { return _comp; }
	
		// TODO: remove
		void	print() { 
			printTree(root()); 
			std::cout << "--------------------------" << "\n";
		}
	
#define RED "\033[0;31m"
#define DEFAULT "\033[0m"

		void	printTree(Node *node, int spaces = 0) const {
			if (!node)
				return ;
			spaces += 8;
			printTree(node->right, spaces);
			std::cout << "\n";
			for (int i = 8; i < spaces; i++)
        		std::cout << " ";
			std::cout << std::setw(2) << (node->black ? "" : RED) << node->value << DEFAULT << "\n";
			printTree(node->left, spaces);
		}
		
		void	r() {
			rotateRight(root());
		}

	protected:
	
		void	rebalance(Node *node) {
			Node* u;
		
			if (node->parent == root())
				root()->black = true;
			if (node->parent->black)
				return ;
			
			if ((u = uncle(node)) && !u->black) {
				node->parent->black = true;
				grandParent(node)->black = false;
				u->black = true;
				rebalance(grandParent(node));
				return ;
			}

			if (leftAligned(node)) {
				rotateRight(grandParent(node));
				node->parent->right->black = false;
				node->parent->black = true;
			}
			else if (rightAligned(node)) {
				rotateLeft(grandParent(node));
				node->parent->left->black = false;
				node->parent->black = true;
			}
			else {
				node->parent->left == node ? rotateRight(node->parent) : rotateLeft(node->parent);
				rebalance(leftOrRightChild(node));
			}
		}

		// Assumes right node exists
		void	rotateLeft(Node* node) {
			Node*	right = node->right;
		
			node->right = right->left;
			if (right->left)
				right->left->parent = node;
			
			parentsChild(node) = right;
			right->left = node;
			right->parent = node->parent;
			node->parent = right;
		}
	
		// Assumes left node exists
		void	rotateRight(Node* node) {
			Node*	left = node->left;
		
			node->left = left->right;
			if (left->right)
				left->right->parent = node;
			
			parentsChild(node) = left;
			left->right = node;
			left->parent = node->parent;
			node->parent = left;
		}
	
		Node*	sentinel() const { return _sentinel; }
		Node*	root() const { return sentinel()->left; }

		Node*	newNode(const T& value, Node* parent) {
			Node*	tmp = _alloc.allocate(1);

			_alloc.construct(tmp, value);
			tmp->parent = parent;
			return tmp;
		}
	
		void	deleteNode(Node* node) {
			_alloc.destroy(node);
			_alloc.deallocate(node, 1);
		}
	
		void	destroySubtree(Node *node) {
			if (!node)
				return ;
			destroySubtree(node->left);
			destroySubtree(node->right);
			deleteNode(node);
		}

		Node*&		leftOrRightChild(Node* node) const { return node->left ? node->left : node->right; }
		size_type	amountOfChildren(Node *node) const { return (node->left ? 1 : 0) + (node->right ? 1 : 0); }
		Node*&		parentsChild(Node* node) const { return node->parent->left == node ? node->parent->left : node->parent->right; }
		Node*		grandParent(Node* node) const { return node->parent->parent; }
		Node*		uncle(Node* node) const { return grandParent(node)->left == node->parent ? grandParent(node)->right : grandParent(node)->left; }
		bool		leftAligned(Node* node) const {	return node->parent->left == node && grandParent(node)->left == node->parent; }
		bool		rightAligned(Node* node) const { return node->parent->right == node && grandParent(node)->right == node->parent; }

		NodeAllocator	_alloc;
		Comp			_comp;
		Node*			_sentinel;
};

} // namespace ft

#endif // RBTREE_HPP
