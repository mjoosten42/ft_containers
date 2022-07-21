#ifndef VECTOR_HPP
#define VECTOR_HPP

	//	https://en.cppreference.com/w/cpp/algorithm

#include <memory> // std::allocator
#include <cstddef> // std::size_t, std::ptrdiff_t

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

			vector(): _capacity(0), _size(0), _data(NULL) {}
			~vector() { _allocator.deallocate(_data, _capacity); };
			explicit	vector(const Allocator& alloc): _allocator(alloc), _capacity(0), _size(0), _data(NULL) {}
			explicit	vector(const vector& rhs): _capacity(0), _size(0), _data(NULL) { *this = rhs; }
	
			//	Initialize with values
			explicit	vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
				: _allocator(alloc), _capacity(n), _size(n), _data(_allocator.allocate(n)) {
				std::fill(begin(), end(), value);
			}

			//	Range constructor
			template <typename InputIt>
			vector(InputIt first, InputIt last, const Allocator& alloc = Allocator(),
				typename enable_if<is_pointer<InputIt>::value, bool>::type = true)
				: _allocator(alloc), _capacity(0), _size(0), _data(NULL) {
				//	TODO: contiguous iterator -> reserve() + std::fill 
				for (; first != last; first++)
					push_back(*first);
			}

			// Copy assignment
			vector&	operator=( const vector& rhs ) {
				if (this != &rhs) {
					if (_capacity != rhs.capacity()) {
						_allocator.deallocate(_data, _capacity);
						_data = rhs.get_allocator().allocate(rhs.capacity());
						_capacity = rhs.capacity();
					}
					_allocator = rhs.get_allocator();
					_size = rhs.size();
					std::copy(rhs.begin(), rhs.end(), begin());
				}
				return *this;
			}

			void	assign(size_type n, const T& value) { // TODO
				(void)n, (void)value;
			}
		
			Allocator	get_allocator() const { return _allocator; }

			//	Element access
		
			reference	at(size_type index) const {
				if (index >= _size)
					throw std::out_of_range("vector");
				return _data[index];
			}
		
			reference	operator[](size_type index) const {
				return _data[index];
			}
					
			reference	front() const { return *begin(); }
			reference	back() const { return *(end() - 1); }
			T*			data() const { return _data; }
		
			//	Iterators

			iterator			begin() const {	return _data; }
			iterator			end() const { return begin() + _size; }
			reverse_iterator	rbegin() const { return end(); } // TODO
			reverse_iterator	rend() const { return begin(); } // TODO

			//	Capacity
	
			bool		empty() const { return _size == 0; }
			size_type	size() const { return _size; }
			size_type	max_size() const { return _allocator.max_size(); }
			size_type	capacity() const { return _capacity; }
		
			void		reserve(size_type n) {
				if (n > _capacity)
					setCapacity(n);
			}

			//	Modifiers

			void	clear() {
				_size = 0;
			}

			iterator	insert(iterator pos, const T& value) {
				insert(pos, 1, value);
				return pos;
			}
	
			void	insert(iterator pos, size_type n, const T& value) {
				shoveRight(pos, n);
				std::fill(pos, pos + n, value);
				_size += n;
			}

			template <typename InputIt>
			void	insert(iterator pos, InputIt first, InputIt last,
			typename enable_if<std::is_pointer<InputIt>::value, bool>::type = true) {
				for (; first != last; first++) {
					insert(pos, *first);
				}
			}

			void	erase( iterator pos) {	// TODO
				(void)pos;
			}
		
			void	push_back(const T& value) {
				if (_size == _capacity)
					setCapacity(_capacity ? _capacity * 2 : 1);
				*end() = value;
				_size++;
			}
			void	pop_back() {
				_size--;
			}

			void	resize(size_type n, T value = T()) {
				reserve(n);
				insert(end(), n - _size, value);
			}

			void	swap(vector& other) {
				std::swap(_allocator, other._allocator);
				std::swap(_capacity, other._capacity);
				std::swap(_size, other._size);
				std::swap(_data, other._data);
			}

		protected:
	
			void	setCapacity(size_type newCapacity) {
				pointer	tmp = _allocator.allocate(newCapacity);
			
				std::copy(begin(), end(), tmp);
				_allocator.deallocate(_data, _capacity);
				_capacity = newCapacity;
				_data = tmp;
			}

			void	shoveRight(iterator pos, size_type n) {
				std::cout << pos << std::endl;
				reserve(_size + n);
				std::move_backward(pos, end(), end() - 1 + n);
			}

		protected:

			Allocator	_allocator;
			size_type	_capacity;
			size_type	_size;
			pointer		_data;
	
	}; // class vector

} // namespace ft

#endif // VECTOR_HPP
