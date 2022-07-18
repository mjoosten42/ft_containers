#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // std::allocator

namespace ft 
{

	template <typename T, class Allocator = std::allocator<T> >
	class vector {

		public:
		
			//	Typedefs
		
			typedef T				value_type;
			typedef Allocator		allocator_type;
			typedef	std::size_t		size_type;
			typedef	std::ptrdiff_t	difference_type;
			typedef	T&				reference;
			typedef	const T&		const_reference;
			typedef	T*				pointer;
			typedef	const T*		const_pointer;
			typedef T*				iterator;
			typedef	const T*		const_iterator;
			typedef	T*				reverse_iterator; // TODO
			typedef	const T*		const_reverse_iterator; // TODO

			//	General member functions
		
			vector(): _begin(NULL), _end(NULL), _capacity(0) {}
			vector( const vector& rhs ) { *this = rhs; }
			~vector() {
				_allocator.deallocate(_begin, _capacity);
			};
			//vector&	operator=( const vector& rhs );
			Allocator	get_allocator() const {
				return _allocator;
			}

			//	Element access
		
			reference	at(size_type index) {
				if (index >= size())
					throw std::out_of_range("vector");
				return _begin[index];
			}
		
			reference	operator[](size_type index) {
				return _begin[index];
			}
					
			reference	front() {
				return *_begin;
			}
		
			reference	back() {
				return *(_end - 1);
			}
		
			//	Iterators
		
			iterator	begin() const {
				return _begin;
			}

			iterator	end() const {
				return _end;
			}

			//	Capacity
	
			bool	empty() const {
				return size() == 0;
			}

			size_type	size() const {
				return _end - _begin;
			}

			size_type	max_size() const {
				return _allocator.max_size();
			}

			void	reserve(size_type n) {
				if (n > _capacity)
					setCapacity(n);
			}

			size_type	capacity() const {
				return _capacity;
			}

			//	Modifiers

			void	push_back(const T& value) {
				if (size() == _capacity)
					setCapacity(_capacity ? _capacity * 2 : 1);
				*_end = value;
				_end++;
			}
			void	pop_back() {
				_end--;
			}


		protected:
	
			void	setCapacity(size_type newCapacity) {
				T*			tmp = _allocator.allocate(newCapacity);
				size_type	newSize = size();
			
				for (size_type i = 0; i < newSize; i++)
					tmp[i] = _begin[i];
				_begin = tmp;
				_end = tmp + newSize;
				_capacity = newCapacity;
			}
	
		protected:

			T*			_begin;
			T*			_end;
			size_type	_capacity;
			Allocator	_allocator;
	
	}; // class vector

} // namespace ft

#endif // VECTOR_HPP
