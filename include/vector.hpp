#ifndef VECTOR_HPP
#define VECTOR_HPP

	//	https://en.cppreference.com/w/cpp/algorithm

#include <memory> // std::allocator
#include <iostream> // << overload

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

			//	Constructors

			//	Default
			vector(): _capacity(0), _begin(NULL), _end(NULL) {}

			//	Custom allocator
			explicit	vector(const Allocator& alloc): _allocator(alloc), _capacity(0), _begin(NULL), _end(NULL) {}
		
			//	Initialize with values
			explicit	vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
				: _allocator(alloc), _capacity(n), _begin(_allocator.allocate(_capacity)), _end(_begin + _capacity) {
				std::fill(_begin, _end, value);
			}

			//	Range constructor
			template <typename InputIterator>
			vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator(),
				typename enable_if<is_pointer<InputIterator>::value, bool>::type = true)
				: _allocator(alloc), _capacity(0), _begin(NULL), _end(NULL) {
				//	TODO: contiguous iterator -> reserve() + std::fill 
				for (; first != last; first++)
					push_back(*first);
			}

			//	Copy constructor
			explicit	vector( const vector& rhs ): _capacity(0), _begin(NULL), _end(NULL) { *this = rhs; }

			//	Destructor
			~vector() {
				_allocator.deallocate(_begin, _capacity);
			};
	
			// Copy assignment
			vector&	operator=( const vector& rhs ) {
				if (this == &rhs)
					return *this;
				_allocator.deallocate(_begin, _capacity);
				_allocator = rhs._allocator;
				_capacity = rhs._capacity;
				_begin = _allocator.allocate(_capacity);
				_end = _begin + rhs.size();
				std::copy(rhs.begin(), rhs.end(), _begin);
				return *this;
			}

			Allocator	get_allocator() const {
				return _allocator;
			}

			//	Element access
		
			reference	at(size_type index) const {
				if (index >= size())
					throw std::out_of_range("vector");
				return _begin[index];
			}
		
			reference	operator[](size_type index) const {
				return _begin[index];
			}
					
			reference	front() const {
				return *_begin;
			}
		
			reference	back() const {
				return *(_end - 1);
			}

			T*	data() const {
				return _begin;
			}
		
			//	Iterators
		
			iterator	begin() const {
				return _begin;
			}

			iterator	end() const {
				return _end;
			}

			reverse_iterator	rbegin() const { // TODO
				return _end;
			}
			
			reverse_iterator	rend() const { // TODO
				return _begin;
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

			void	clear() {
				_end = _begin;
			}

			void	insert(iterator pos, const T& value) { // TODO
				*pos = value;
			}
	
			void	erase( iterator pos) {	// TODO
				(void)pos;
			}
		
			void	push_back(const T& value) {
				if (size() == _capacity)
					setCapacity(_capacity ? _capacity * 2 : 1);
				*_end = value;
				_end++;
			}
			void	pop_back() {
				_end--;
			}

			void	resize(size_type n, T value = T()) {
				(void)n, (void)value;
			}

			void	swap(vector& other) { // TODO: switch pointers
				vector	tmp = *this;

				*this = other;
				other = tmp;
			}

		protected:
	
			void	setCapacity(size_type newCapacity) {
				T*			tmp = _allocator.allocate(newCapacity);
				size_type	newSize = size();
			
				std::copy(_begin, _end, tmp);
				_capacity = newCapacity;
				_begin = tmp;
				_end = tmp + newSize;
			}

		protected:

			Allocator	_allocator;
			size_type	_capacity;
			pointer		_begin;
			pointer		_end;
	
	}; // class vector

	template <typename T>
	static void	printOne(const T& value) {
		std::cout << value << " ";
	}

} // namespace ft

#endif // VECTOR_HPP
