#ifndef REDBLACKTREE_HPP
#define REDBLACKTREE_HPP

#define RED 0
#define BLACK 1

namespace ft
{

template <typename T>
class redBlackTree {
	public:
		redBlackTree() {};
		redBlackTree(const redBlackTree& rhs) { *this = rhs; }
		~redBlackTree() {};
		redBlackTree&	operator=(const redBlackTree& rhs) { (void)rhs; };
		
	private:
		struct Node {
			T		content;
			Node*	left;
			Node*	right;
			char	color;
		};
};

}

#endif // REDBLACKTREE_HPP
