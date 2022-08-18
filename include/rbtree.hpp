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
#include <cmath>  // TODO: remove

namespace ft
{

#define LEFT 0
#define RIGHT 1

#define UNALIGNED 2

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
		os << "{ " << node.value;
		os << ", parent: "; node.parent ? os << node.parent->value : os << "-";
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

template <typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T> >
class rbtree {
	protected:

		typedef Node<T>												Node;
		typedef typename Allocator::template rebind<Node>::other	NodeAllocator;

	public:

		// Typedefs

		typedef std::size_t							size_type;
		typedef std::ptrdiff_t						difference_type;
		typedef Compare								key_compare;
		typedef Allocator							allocator_type;
		typedef T&									reference;
		typedef const T&							const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
		typedef rbtreeIterator<T>					iterator;
		typedef rbtreeIterator<const T>				const_iterator;
		typedef reverseIterator<iterator>			reverse_iterator;
		typedef reverseIterator<const_iterator>		const_reverse_iterator;
		
		rbtree(): _alloc(), _comp(), _sentinel(newNode(T(), NULL)), _size(0) {}
		rbtree(const rbtree& rhs): _sentinel(NULL) { *this = rhs; }

		explicit	rbtree(const Compare& comp, const Allocator& alloc = Allocator())
			: _alloc(alloc), _comp(comp), _sentinel(newNode(T(), NULL)), _size(0) {}

		template <class InputIt>
		explicit	rbtree(InputIt first, InputIt last, const Compare& comp = Compare(), const Allocator& alloc = Allocator())
			: _alloc(alloc), _comp(comp), _sentinel(newNode(T(), NULL)), _size(0) {
				insert(first, last);
			}

		~rbtree() {
			destroySubtree(root());
			deleteNode(sentinel());
		};

		rbtree&	operator=(const rbtree& rhs) {
			if (sentinel())
				destroySubtree(sentinel());
			_alloc = rhs.get_allocator();
			_comp = rhs.key_comp();
			_sentinel = newNode(T(), NULL);
			_size = 0;
			insert((const_cast<rbtree&>(rhs)).begin(), (const_cast<rbtree&>(rhs)).end()); // TODO
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

		void	swap(rbtree& other) {
			std::swap(_alloc, other._alloc);
			std::swap(_comp, other._comp);
			std::swap(sentinel(), other.sentinel());
		}

		pair<iterator, bool>	insert(const T& value) {
			Node*	tmp;
			Node*	p = root();
			Node*	parent = sentinel();

			while (p) {
				parent = p;
				if (_comp(value, p->value))
					p = p->left;
				else if (_comp(p->value, value))
					p = p->right;
				else
					return make_pair<iterator, bool>(p, false);
			}
			tmp = newNode(value, parent);
			if (parent == sentinel())
				root() = tmp;
			else
				(*parent)[!_comp(value, parent->value)] = tmp;
			rebalanceInsertion(parent);
			_size++;
			return make_pair<iterator, bool>(tmp, true);
		}

		// TODO: use hint
		iterator	insert(iterator hint, const T& value) {
			return insert(value).first;
			(void)hint;
		}
	
		template <typename InputIt>
		void	insert(InputIt first, InputIt last) {
			for (; first != last; first++)
				insert(*first);
		}

		void	erase(iterator pos) {
			Node*	p = pos;
			Node*	q;

			switch (nbChildren(p)) {
			case 0:
				rebalanceDeletion(p);
				parentMyRef(p) = NULL;
				break;
			case 1:
				rebalanceDeletion(p);
				myChildRef(p)->parent = p->parent;
				parentMyRef(p) = myChildRef(p);
				break;
			case 2:
				q = ++iterator(p);
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
				it--;
				std::cout << "first: " << *(Node*)first << "\n";
				std::cout << "it: " << *it << "\n";
				erase(it);
				print();
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
		
		pair<iterator, iterator>
					equal_range(const T& value) { return make_pair(lower_bound(value), upper_bound(value)); }
		iterator	lower_bound(const T& value) { return std::lower_bound(begin(), end(), value, _comp); }
		iterator	upper_bound(const T& value) { return std::upper_bound(begin(), end(), value, _comp); }
	
		pair<const_iterator, const_iterator>
						equal_range(const T& value) const { return make_pair(lower_bound(value), upper_bound(value)); }
		const_iterator	lower_bound(const T& value) const { return std::lower_bound(begin(), end(), value, _comp); }
		const_iterator	upper_bound(const T& value) const { return std::upper_bound(begin(), end(), value, _comp); }

		// Observers

		key_compare		key_comp() const { return _comp; }

		// TODO: remove
#define RED "\033[0;31m"
#define DEFAULT "\033[0m"
#define SPACES 16

		void	print() {
			float	len = SPACES;
		
			printTree(root());

			if (!empty())
				len *=  M_SQRT2 * std::log2(size());
			std::cout << std::string((int)len, '-') << "\n";
		}
	

		void	printTree(Node *node, int spaces = 0) const {
			if (!node)
				return ;
			spaces += SPACES;
			printTree(node->right, spaces);
			std::cout << "\n";
			for (int i = SPACES; i < spaces; i++)
        		std::cout << " ";
			std::cout << std::setw(2) << (node->black ? DEFAULT : RED) << node->value << DEFAULT << "\n";
			printTree(node->left, spaces);
		}

	protected:

		// https://adtinfo.org/libavl.html/Inserting-an-RB-Node-Step-3-_002d-Rebalance.html

		// p is parent of newly inserted node
		void	rebalanceInsertion(Node *p) {
			root()->black = true;
			if (p->black || p == sentinel())
				return ;
			
			// 1: sibling is red
			// Push grandparents blackness down and recurse for grandparent
			Node* 	sib = sibling(p);
			if (sib && !sib->black) {
				sib->black = true;
				p->black = true;
				p->parent->black = false;
				rebalanceInsertion(p->parent->parent);
				return ;
			}

			int	side = parentsSide(p);
		
			// 3: red nodes are unaligned
			// Align by rotating parent
			if ((*p)[!side] && !(*p)[!side]->black) {
				rotate(p, side);
				p = p->parent;
			}

			// 2: red nodes are aligned
			// Rotate black grandparent and swap colors
			rotate(p->parent, !side);
			(*p)[!side]->black = false;
			p->black = true;
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

			int	siblingSide = parentsSide(sib);

			// 5: siblings near child is red and far child is black
			// Swap siblings and red childs color. Rotate sibling towards p and recurse
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
			
			parentMyRef(p) = q;
			(*q)[dir] = p;
			q->parent = p->parent;
			p->parent = q;
		}

		Node*&	sentinel() { return _sentinel; }
		Node*&	root() { return sentinel()->left; }

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

		Node*	sibling(Node* node) const { return node->parent->left == node ? node->parent->right : node->parent->left; }
		Node*&	myChildRef(Node* node) const { return node->left ? node->left : node->right; }
		Node*&	parentMyRef(Node* node) const { return node->parent->left == node ? node->parent->left : node->parent->right; }
		
		bool	isBlack(Node* node) const { return !node || node->black; }
		int		nbChildren(Node *node) const { return !!node->left + !!node->right; }
		int		parentsSide(Node* node) const { return node->parent->left == node ? LEFT : RIGHT; }

		NodeAllocator	_alloc;
		Compare			_comp;
		Node*			_sentinel;
		size_type		_size;
};

} // namespace ft

namespace std {
	template <typename T, typename Compare, typename Allocator>
	void	swap(ft::rbtree<T, Compare, Allocator>& lhs, ft::rbtree<T, Compare, Allocator>& rhs) { lhs.swap(rhs); }
}

#endif // RBTREE_HPP
