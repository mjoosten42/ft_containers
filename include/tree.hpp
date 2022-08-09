#ifndef TREE_HPP
#define TREE_HPP

	// https://www.programiz.com/dsa/insertion-in-a-red-black-tree
	// https://adtinfo.org/libavl.html/Deleting-from-a-BST.html (erase)

#include <functional> // std::less
#include <memory> // std::allocator
#include <iterator> // bidirectional_iterator_tag
#include "iterator.hpp" // reverseIterator
#include "pair.hpp"
#include <iostream> // TODO: remove
#include <iomanip> // TODO: remove

// Get the parent's pointer to this node
#define PARENTS_CHILD(node) node->parent->left == node ? node->parent->left : node->parent->right

// Get this node's left or right child depending on which one exists
#define LEFT_OR_RIGHT_CHILD(node) node->left ? node->left : node->right

#define AMOUNT_OF_CHILDREN(node) node->left ? node->right ? 2 : 1 : node->right ? 1 : 0

// Compare a node with a value
#define COMPARE(node, value) _comp(value, node->value) ? node->left : node->right 

namespace ft
{

template <typename T>
struct Node {
	T		value;
	Node*	parent;
	Node*	left;
	Node*	right;

	Node(const T& value): value(value), left(NULL), right(NULL) {};

	// TODO: remove
	friend std::ostream&	operator<<(std::ostream& os, const Node& node) {
		os << "Node: { " << node.value;
		os << ", " << node.parent;
		os << ", " << node.left;
		os << ", " << node.right;
		os << " }";
		return os;
	}
};

template <typename T>
struct treeIterator {
		typedef Node<T>	Node;
	public:
		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef T								value_type;
		typedef std::ptrdiff_t					difference_type;
		typedef T*								pointer;
		typedef T&								reference;
	
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

		bool	operator==(const treeIterator& rhs) { return _p == rhs._p; }
		bool	operator!=(const treeIterator& rhs) { return _p != rhs._p; }

		operator Node*() const { return _p; }

	private:
		Node*	_p;
};

template <typename T, typename Comp = std::less<T>, typename Allocator = std::allocator<T> >
class tree {

		typedef Node<T>	Node;
		typedef typename Allocator::template rebind<Node>::other	NodeAllocator;

	public:

		// Typedefs

		typedef std::size_t					size_type;
		typedef treeIterator<T>				iterator;
		typedef reverseIterator<iterator>	reverse_iterator;
		
		tree(): _alloc(), _comp(), _sentinel(newNode()) {};
		tree(const tree& rhs): _sentinel(NULL) { *this = rhs; }

		~tree() {
			clear();
			deleteNode(sentinel());
		};

		tree&	operator=(const tree& rhs) {
			if (sentinel())
				destroySubtree(sentinel());
			_alloc = rhs.get_allocator();
			_comp = rhs.value_comp();
			_sentinel = newNode();
			insert(rhs.begin(), rhs.end());
			return *this;
		}

		Allocator	get_allocator() const { return _alloc; }

		// Element access

		T&	at(const T& value) {
			iterator it = find(value);

			if (it == end())
				throw std::out_of_range("map");
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

		void	swap(tree& other) {
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
				(COMPARE(q, value)) = tmp;
			return make_pair<iterator, bool>(tmp, true);
		}
	
		template <typename InputIt>
		void	insert(InputIt first, InputIt last) {
			for (; first != last; first++)
				insert(*first);
		}

		void	erase(iterator pos) {
			Node*	node = pos;

			switch (AMOUNT_OF_CHILDREN(node)) {
				case 0:
					(PARENTS_CHILD(node)) = NULL;
					deleteNode(node);
					break;
				case 1:
					(LEFT_OR_RIGHT_CHILD(node))->parent = node->parent;
					(PARENTS_CHILD(node)) = LEFT_OR_RIGHT_CHILD(node);
					deleteNode(node);
					break;
				case 2:
					Node*	successor = node->right;

					while (successor->left)
						successor = successor->left;
					std::swap(node->value, successor->value);
					erase(successor);
			}
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
			std::cout << "--------------------------" << std::endl;
		}
	
		void	printTree(Node *node, int spaces = 0) const {
			if (!node)
				return ;
			spaces += 8;
			printTree(node->right, spaces);
			std::cout << std::endl;
			for (int i = 8; i < spaces; i++)
        		std::cout << " ";
			std::cout << std::setw(2) << node->value << std::endl;
			printTree(node->left, spaces);
		}
		

	private:
	
		Node*	sentinel() const { return _sentinel; }
		Node*	root() const { return sentinel()->left; }

		Node*	newNode(const T& value = T(), Node* parent = NULL) {
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

		NodeAllocator	_alloc;
		Comp			_comp;
		Node*			_sentinel;
};

} // namespace ft

#endif // TREE_HPP
