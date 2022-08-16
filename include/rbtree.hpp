#ifndef RBTREE_HPP
#define RBTREE_HPP

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

#define LEFT 0
#define RIGHT 1

template <typename T>
struct Node {
	T		value;
	Node*	parent;
	Node*	left;
	Node*	right;
	bool	black;

	Node(const T& value): value(value), left(NULL), right(NULL), black(false) {}

	Node*&	operator[](int dir) { return dir ? right : left; }

	// TODO: remove
	friend std::ostream&	operator<<(std::ostream& os, const Node& node) {
		os << " { " << node.value;
		os << ", parent: " << node.parent->value;
		os << ", left: "; node.left ? os << node.left->value : os << "-";
		os << ", right: "; node.right ? os << node.right->value : os << "-";
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
		
			if ((*_p)[dir]) {
				_p = (*_p)[dir];
				while ((*_p)[!dir])
					_p = (*_p)[!dir];
			}
			else {
				_p = _p->parent;
				while (q == (*_p)[dir]) {
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

		// Implicit conversion
		operator Node*() const { return _p; }

	private:
		Node*	_p;
};

template <typename T, typename Comp = std::less<T>, typename Allocator = std::allocator<T> >
class rbtree {
	protected:

		typedef Node<T>												Node;
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
			_size = 0;
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

			while (p->left)
				p = p->left;
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
			root() = NULL;
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
					p = p->left;
				else if (_comp(p->value, value))
					p = p->right;
				else
					return make_pair<iterator, bool>(p, false);
			}
			tmp = newNode(value, q);
			if (q == sentinel())
				root() = tmp;
			else
				(*q)[!_comp(value, q->value)] = tmp;
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
			Node*	p = pos;
			Node*	q = p->right;

			switch (nbChildren(p)) {
			case 0:
				rebalanceDeletion(p);
				parentsChild(p) = NULL;
				break;
			case 1:
				rebalanceDeletion(p);
				leftOrRightChild(p)->parent = p->parent;
				parentsChild(p) = leftOrRightChild(p);
				break;
			case 2:
				while (q->left)
					q = q->left;
				std::swap(p->value, q->value); // TODO: keep iterator valid
				return erase(q);
			}
			deleteNode(p);
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

		// Map and set store unique keys, so at most 1 element
		size_type	count(const T& value) const {
			return find(value) != end();
		}
	
		iterator	find(const T& value) {
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

		iterator	lower_bound(const T& value) {
			return std::lower_bound(begin(), end(), value, _comp);
		}
	
		iterator	upper_bound(const T& value) {
			return std::upper_bound(begin(), end(), value, _comp);
		}
	
		// Observers

		Comp	value_comp() const { return _comp; }
	
		// TODO: remove
		void	print() { 
			printTree(root()); 
			std::cout << "--------------------------------------------------------------------------------" << "\n";
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

	protected:

		// https://adtinfo.org/libavl.html/Inserting-an-RB-Node-Step-3-_002d-Rebalance.html

		void	rebalanceInsertion(Node *p) {
			root()->black = true;
			if (p->parent->black || p == root())
				return ;

			// 1: uncle is red
			// Push grandparents blackness down
			Node* 	u = uncle(p);
			if (u && !u->black) {
				u->black = true;
				p->parent->black = true;
				grandParent(p)->black = false;
				rebalanceInsertion(grandParent(p));
				return ;
			}

			// 2: red nodes are aligned
			// Rotate grandparent
			int	a = alignment(p);
			if (a == LEFT || a == RIGHT) {
				rotate(grandParent(p), !a);
				(*p->parent)[!a]->black = false;
				p->parent->black = true;
			}
		
			// 3: red nodes are unaligned
			// Align and recurse
			else {				
				rotate(p->parent, !parentsSide(p));
				rebalanceInsertion(leftOrRightChild(p));
			}
		}

		// https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea

		void	rebalanceDeletion(Node* p) {
			if (!p->black || p == root())
				return ;

			Node*	sib = sibling(p);
			
			// 4: sibling is red
			// Swap siblings and parents color. Rotate parent towards p and recurse
			if (!sib->black) {
				p->parent->black = false;
				sib->black = true;
				rotate(p->parent, sib->parent->left == sib ? RIGHT : LEFT);
				rebalanceDeletion(p);
				return ;
			}

			// 3: sibling is black and has no red children
			// Push blackness up and recurse
			if (isBlack(sib->left) && isBlack(sib->right)) {
				sib->black = false;
				if (p->parent->black)
					rebalanceDeletion(p->parent);
				else
					p->parent->black = true;
				return ;
			}

			// 5: siblings near child is red and far child is black
			// Swap siblings and red childs color. Rotate sibling towards p and recurse
			int	siblingSide = parentsSide(sib);
			if (isBlack((*sib)[siblingSide]) && !(*sib)[!siblingSide]->black) {
				sib->black = false;
				(*sib)[!siblingSide]->black = true;
				rotate(sib, siblingSide);
				rebalanceDeletion(p);
				return ;
			}

			// 6: sibling's far child is red
			// Swap sibling and parents color. Rotate parent towards p. Color red child black
			std::swap(p->parent->black, sib->black);
			rotate(p->parent, !siblingSide);
			(*sib)[siblingSide]->black = true;
		}
	
		void	rotate(Node* p, int dir) {
			Node*	q = (*p)[!dir];

			(*p)[!dir] =(*q)[dir];
			if ((*q)[dir])
				(*q)[dir]->parent = p;
			
			parentsChild(p) = q;
			(*q)[dir] = p;
			q->parent = p->parent;
			p->parent = q;
		}

		Node*&	sentinel() { return _sentinel; }
		Node*&	root() { return sentinel()->left; }
		Node*&	root() const { return sentinel()->left; }

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

		Node*	uncle(Node* node) const { return grandParent(node)->left == node->parent ? grandParent(node)->right : grandParent(node)->left; }
		Node*	sibling(Node* node) const { return node->parent->left == node ? node->parent->right : node->parent->left; }
		Node*	grandParent(Node* node) const { return node->parent->parent; }
		Node*&	parentsChild(Node* node) const { return node->parent->left == node ? node->parent->left : node->parent->right; }
		Node*&	leftOrRightChild(Node* node) const { return node->left ? node->left : node->right; }
		
		bool	isBlack(Node* node) const { return !node || node->black; }
		int		alignment(Node* node) const { return aligned(node, LEFT) ? LEFT : aligned(node, RIGHT) ? RIGHT : -1; }
		int		nbChildren(Node *node) const { return !!node->left + !!node->right; }
		int		parentsSide(Node* node) const { return node->parent->left == node ? LEFT : RIGHT; }
		bool	aligned(Node* node, int dir) const { return (*node->parent)[dir] == node && (*grandParent(node))[dir] == node->parent;}

		NodeAllocator	_alloc;
		Comp			_comp;
		Node*			_sentinel;
		size_type		_size;
};

} // namespace ft

#endif // RBTREE_HPP
