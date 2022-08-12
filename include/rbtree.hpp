#ifndef RBTREE_HPP
#define RBTREE_HPP

	// https://www.programiz.com/dsa/insertion-in-a-red-black-tree
	// https://adtinfo.org/libavl.html/RB-Balancing-Rule.html
	// https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
	// https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea

#include <functional> // std::less
#include <memory> // std::allocator
#include <iterator> // bidirectional_iterator_tag
#include "iterator.hpp" // reverseIterator
#include "pair.hpp"
#include <iostream> // TODO: remove
#include <iomanip> // TODO: remove
#include <initializer_list>

namespace ft
{

#define LEFT 0
#define RIGHT 1

template <typename T>
struct Node {
	T		value;
	Node*	parent;
	Node*	child[2];
	bool	black;

	Node(const T& value): value(value), black(false) { child[LEFT] = NULL; child[RIGHT] = NULL; }

	// TODO: remove
	friend std::ostream&	operator<<(std::ostream& os, const Node& node) {
		os << &node;
		os << " { " << node.value;
		os << ", " << node.child[LEFT];
		os << ", " << node.parent;
		os << ", " << node.child[RIGHT];
		os << ", " << (node.black ? "black" : "red");
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
		rbtreeIterator(const rbtreeIterator& rhs): _p(rhs._p) {};
		rbtreeIterator&	operator=(const rbtreeIterator& rhs) { _p = rhs._p; return *this; }
	
		T&  operator*() const { return _p->value; }
		T&  operator->() const { return &_p->value; }

		rbtreeIterator&	iterate(int dir) {
			Node*	q = _p;
		
			if (_p->child[dir]) {
				_p = _p->child[dir];
				while (_p->child[!dir])
					_p = _p->child[!dir];
			}
			else {
				_p = _p->parent;
				while (q == _p->child[dir]) {
					q = _p;
					_p = _p->parent;
				}
			}
			return *this;
		}
		rbtreeIterator&	operator++() { return iterate(RIGHT); }
		rbtreeIterator&	operator--() { return iterate(LEFT); }

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

		typedef std::size_t						size_type;
		typedef rbtreeIterator<T>				iterator;
		typedef rbtreeIterator<const T>			const_iterator;
		typedef reverseIterator<iterator>		reverse_iterator;
		typedef reverseIterator<const_iterator>	const_reverse_iterator;
		
		rbtree(): _alloc(), _comp(), _sentinel(newNode(T(), NULL)), _size(0) {}
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
			_size = rhs.size();
			insert(rhs.begin(), rhs.end());
			return *this;
		}

		Allocator	get_allocator() const { return _alloc; }

		// Element access

		T&	at(const T& value) {
			iterator it = find(value);

			if (it == end())
				throw std::out_of_range("rbtree");
			return *it;
		}
	
		T&	operator[](const T& value) {
			iterator it = find(value);
		
			if (it == end())
				return *insert(value).first;
			return *it;
		}

		// Iterators

		iterator	begin() {
			Node*	p = sentinel();

			while (p->child[LEFT])
				p = p->child[LEFT];
			return p;
		}

		iterator			end() { return sentinel(); }
		reverse_iterator	rbegin() { return end(); }
		reverse_iterator	rend() { return begin(); }
	
		// Capacity

		bool		empty() const { return size() == 0; }
		size_type	size() const { return _size; }
		size_type	max_size() const { return _alloc.max_size(); }
	
		// Modifiers

		void	clear() {
			destroySubtree(root());
			sentinel()->child[LEFT] = NULL;
			_size = 0;
		}

		void	swap(rbtree& successor) {
			std::swap(_alloc);
			std::swap(_comp);
			std::swap(sentinel(), successor.sentinel());
		}

		pair<iterator, bool>	insert(const T& value) {
			Node*	tmp;
			Node*	p = root();
			Node*	q = sentinel();

			while (p) {
				q = p;
				if (_comp(value, p->value))
					p = p->child[LEFT];
				else if (_comp(p->value, value))
					p = p->child[RIGHT];
				else
					return make_pair<iterator, bool>(p, false);
			}
			tmp = newNode(value, q);
			if (q == sentinel())
				root() = tmp;
			else
				q->child[!_comp(value, q->value)] = tmp;
			_size++;
			rebalanceInsertion(tmp);
			return make_pair<iterator, bool>(tmp, true);
		}
	
		template <typename InputIt>
		void	insert(InputIt first, InputIt last) {
			for (; first != last; first++)
				insert(*first);
		}

		void	erase(iterator pos) {
			Node*	node = pos;

			switch (nbChildren(node)) {
			case 0:
				parentsChild(node) = NULL;
				break;
			case 1:
				leftOrRightChild(node)->parent = node->parent;
				parentsChild(node) = leftOrRightChild(node);
				break;
			case 2:
				Node*	successor = node->child[RIGHT];

				while (successor->child[LEFT])
					successor = successor->child[LEFT];
				std::swap(node->value, successor->value); // TODO
				return erase(successor);
			}
			rebalanceDeletion(node);
			deleteNode(node);
			_size--;
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
			return find(value) != end();
		}
	
		iterator	find(const T& value) {
			Node*	p = root();
	
			while (p) {
				if (_comp(value, p->value))
					p = p->child[LEFT];
				else if (_comp(p->value, value))
					p = p->child[RIGHT];
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
			printTree(node->child[RIGHT], spaces);
			std::cout << "\n";
			for (int i = 8; i < spaces; i++)
        		std::cout << " ";
			std::cout << std::setw(2) << (node->black ? "" : RED) << node->value << DEFAULT << "\n";
			printTree(node->child[LEFT], spaces);
		}
	
	protected:
	
		void	rebalanceInsertion(Node *node) {
			root()->black = true;
			if (node->parent->black || node == root())
				return ;
			
			Node* 	u = uncle(node);
			if (u && !u->black) {
				node->parent->black = true;
				grandParent(node)->black = false;
				u->black = true;
				rebalanceInsertion(grandParent(node));
				return ;
			}

			int	a = alignment(node);
			if (a == LEFT || a == RIGHT) {
				rotate(grandParent(node), !a);
				node->parent->child[!a]->black = false;
				node->parent->black = true;
			}
			else {
				node->parent->child[LEFT] == node ? rotate(node->parent, RIGHT) : rotate(node->parent, LEFT);
				rebalanceInsertion(leftOrRightChild(node));
			}
		}

		void	rebalanceDeletion(Node* node) {
			if ((!node->black && !nbChildren(node)) || node == root())
				return ;

			Node*	sib = sibling(node);
			if (sib && sib->black && (!sib->child[LEFT] || sib->child[RIGHT]->black)) {
				sib->black = false;
			}
			
		}
	
		void	rotate(Node* node, int dir) {
			Node*	q = node->child[!dir];

			node->child[!dir] = q->child[dir];
			if (q->child[dir])
				q->child[dir]->parent = node;
			
			parentsChild(node) = q;
			q->child[dir] = node;
			q->parent = node->parent;
			node->parent = q;
		}

		Node*&	sentinel() { return _sentinel; }
		Node*&	root() { return sentinel()->child[LEFT]; }
		Node*&	root() const { return sentinel()->child[LEFT]; }

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
			destroySubtree(node->child[LEFT]);
			destroySubtree(node->child[RIGHT]);
			deleteNode(node);
		}

		size_type	nbChildren(Node *node) const { return (node->child[LEFT] ? 1 : 0) + (node->child[RIGHT] ? 1 : 0); }
		Node*&		leftOrRightChild(Node* node) const { return node->child[LEFT] ? node->child[LEFT] : node->child[RIGHT]; }
		Node*&		parentsChild(Node* node) const { return node->parent->child[LEFT] == node ? node->parent->child[LEFT] : node->parent->child[RIGHT]; }
		Node*		grandParent(Node* node) const { return node->parent->parent; }
		Node*		sibling(Node* node) const { return node->parent->child[LEFT] == node ? node->parent->child[RIGHT] : node->parent->child[LEFT]; }
		Node*		uncle(Node* node) const { return grandParent(node)->child[LEFT] == node->parent ? grandParent(node)->child[RIGHT] : grandParent(node)->child[LEFT]; }
		bool		aligned(Node* node, int dir) const { return node->parent->child[dir] == node && grandParent(node)->child[dir] == node->parent;}
		int			alignment(Node* node) const { return aligned(node, LEFT) ? LEFT : aligned(node, RIGHT) ? RIGHT : -1; }

		NodeAllocator	_alloc;
		Comp			_comp;
		Node*			_sentinel;
		size_type		_size;
};

} // namespace ft

#endif // RBTREE_HPP
