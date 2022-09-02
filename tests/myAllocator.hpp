#ifndef MYALLOCATOR_HPP
#define MYALLOCATOR_HPP

#include <limits>

template <typename T>
class myAllocator {
	public:
		typedef T				value_type;
		typedef T*				pointer;
		typedef const T*		const_pointer;
		typedef T&				reference;
		typedef const T&		const_reference;
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;

		myAllocator(): mMaxSize(std::numeric_limits<size_type>::max() / sizeof(value_type)) {}
		myAllocator(size_type n): mMaxSize(n) {}
		myAllocator(const myAllocator& other): mMaxSize(other.max_size()) {}

		template <typename U>
		myAllocator(const myAllocator<U>& other): mMaxSize(other.max_size()) {}

		pointer	allocate(size_type n) { return malloc(n * sizeof(T)); }
		void	deallocate(pointer p, size_type n) { free(p); (void)n; }
		void	construct(pointer p, const T& value) { new (p) T(value); }
		void	destroy(pointer p) { p->~T(); }

		size_type	max_size() const { return mMaxSize; }
		void		set_size(size_type size) { mMaxSize = size; }

		friend bool operator==(const myAllocator&, const myAllocator&) { return true; }
		friend bool operator!=(const myAllocator&, const myAllocator&) { return false; }
	
	private:
		size_type mMaxSize;
};

#endif // MYALLOCATOR_HPP
