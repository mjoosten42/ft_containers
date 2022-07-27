#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // std::allocator
#include <cstddef> // std::size_t, std::ptrdiff_t
#include "iterator.hpp" // iterator, contigousindexterator_tag
#include "meta.hpp" // enable_if, is_pointer // TODO

namespace ft 
{

template <typename T, class Allocator = std::allocator<T> >
class vector {

	public:
	
		//	Forward declaration
		class vectorIterator;
	
		//	Typedefs
	
		typedef T				value_type;
		typedef Allocator		allocator_type;
		typedef	std::size_t		size_type;
		typedef	std::ptrdiff_t	difference_type;
		typedef	T&				reference;
		typedef	const T&		const_reference;
		typedef	T*				pointer;
		typedef	const T*		const_pointer;
		typedef vectorIterator	iterator;
		typedef	const T*		const_iterator;
		typedef	T*				reverse_iterator; // TODO
		typedef	const T*		const_reverse_iterator; // TODO

		//	Constructors
	
		vector(): _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const Allocator& alloc): _allocator(alloc), _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const vector& rhs): _capacity(0), _size(0), _data(NULL) { *this = rhs; }

		~vector() {
			clear();
			_allocator.deallocate(_data, _capacity);
		};
	
		//	Initialize with values
		explicit	vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
			: _allocator(alloc), _capacity(n), _size(0), _data(_allocator.allocate(n)) {
			assign(n, value);
		}

		//	Range constructor
		template <typename InputIt>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator(),
			typename enable_if<is_pointer<InputIt>::value, bool>::type = true)
			: _allocator(alloc), _capacity(0), _size(0), _data(NULL) {
			assign(first, last);
		}

		// Copy assignment
		vector&	operator=( const vector& rhs ) {
			if (this == &rhs)
				return (*this);
			if (_capacity < rhs.size()) {
				clear();
				_allocator.deallocate(_data, _capacity);
				_data = rhs.get_allocator().allocate(rhs.capacity());
				_capacity = rhs.capacity();
			}
			_allocator = rhs.get_allocator();
			_size = rhs.size();
			std::copy(rhs.begin(), rhs.end(), begin());
			return *this;
		}

		void	assign(size_type n, const T& value) {
			reserve(n);
			std::fill_n(begin(), n, value);
			_size = n;
		}

		template <typename InputIt>
		void	assign(InputIt first, InputIt last,
		typename enable_if<std::is_pointer<InputIt>::value, bool>::type = true) {
			clear();
			for (; first != last; first++)
				push_back(*first);
		}
	
		allocator_type	get_allocator() const { return _allocator; }

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
		pointer		data() const { return _data; }
	
		//	Iterators

		iterator			begin() const {	return iterator(*this, 0); }
		iterator			end() const { return iterator(*this, _size); }
		reverse_iterator	rbegin() const { return end(); } // TODO
		reverse_iterator	rend() const { return begin(); } // TODO

		//	Capacity

		bool		empty() const { return _size == 0; }
		size_type	size() const { return _size; }
		size_type	max_size() const { return _allocator.max_size(); }
		size_type	capacity() const { return _capacity; }
	
		void	reserve(size_type n) {
			if (n > max_size())
				throw std::length_error("vector");
			if (n > _capacity)
				setCapacity(n);
		}

		//	Modifiers

		void	clear() {
			for (iterator it = begin(); it != end(); it++)
				_allocator.destroy(&*it);
			_size = 0;
		}

		iterator	insert(iterator pos, const T& value) {
			insert(pos, 1, value);
			return pos;
		}

		void	insert(iterator pos, size_type n, const T& value) {			
			shoveRight(pos, n);
			std::fill_n(pos, n, value);
			_size += n;
		}

		template <typename InputIt>
		void	insert(iterator pos, InputIt first, InputIt last,
		typename enable_if<std::is_pointer<InputIt>::value, bool>::type = true) {
			for (; first != last; first++)
				insert(begin() + pos++, *first);
		}

		iterator	erase(iterator pos) {
			std::move(pos + 1, end(), pos);
			_size--;
			return pos;
		}
	
		iterator	erase(iterator first, iterator last) {
			std::move(last, end(), first);
			_size -= last - first;
			return end();
		}
	
		void	push_back(const T& value) {
			if (_size == _capacity)
				setCapacity(_capacity ? _capacity * 2 : 1);
			_allocator.construct(&*end(), value);
			_size++;
		}

		void	pop_back() {
			_allocator.destroy(&*(end() - 1));
			_size--;
		}

		void	resize(size_type n, T value = T()) {
			reserve(n);
			for (; n < _size; n++)
				_allocator.destroy(begin() + n);
			if (n > _size)
				insert(end(), n - _size, value);
			_size = n;
		}

		void	swap(vector& other) {
			std::swap(_allocator, other._allocator);
			std::swap(_capacity, other._capacity);
			std::swap(_size, other._size);
			std::swap(_data, other._data);
		}
		
		// Iterator
	
		class vectorIterator: public ft::iterator<contigous_iterator_tag, T> {
			public:
				vectorIterator(const vector& v, size_type n): _v(&v), _index(n) {}
				vectorIterator(const vectorIterator& rhs) { *this = rhs; }
				vectorIterator&	operator=(const vectorIterator& other) { _v = other._v; _index = other._index; return *this; }
			
				vectorIterator&	operator++() {
					_index++;
					return *this;
				}
			
				vectorIterator	operator++(int) {
					vectorIterator	tmp = *this;
					++*this;
					return tmp;
				}
			
				vectorIterator&	operator--() {
					_index--;
					return *this;
				}
			
				vectorIterator	operator--(int) {
					vectorIterator	tmp = *this;
					--*this;
					return tmp;
				}

				reference	operator*() const { return (*_v)[_index]; }

				vectorIterator	operator+(size_type n) {
					return vectorIterator(*_v, _index + n);
				}
			
				vectorIterator	operator+(const vectorIterator& rhs) {
					return vectorIterator(*_v, _index + rhs._index);
				}
			
				size_type	operator-(const vectorIterator& rhs) {
					return _index - rhs._index;
				}
			
			
				bool	operator==(const vectorIterator& rhs) const { return _index == rhs._index; }
				bool	operator!=(const vectorIterator& rhs) const { return !(*this == rhs); }

			private:
				const vector*	_v;
				std::size_t		_index;
		};
		
		friend class vectorIterator;
	
	protected:

		void	setCapacity(size_type newCapacity) {
			pointer	tmp = _allocator.allocate(newCapacity);

			std::move(begin(), end(), tmp);
			_allocator.deallocate(_data, _capacity);
			_capacity = newCapacity;
			_data = tmp;
		}

		void	shoveRight(iterator pos, size_type n) {
			difference_type	index = pos - begin(); // Save index in case of reallocation

			reserve(_size + n);
			std::move_backward(begin() + index, end(), end() + n);
		}

	protected:

		Allocator	_allocator;
		size_type	_capacity;
		size_type	_size;
		pointer		_data;

	friend std::ostream&	operator<<(std::ostream& os, const vector<T>& v) {
		std::cout << "size: " << std::setw(2) << v.size();
		std::cout << ", cap: " << std::setw(2) << v.capacity();
		std::cout << " { ";
		for (size_t i = 0; i < v.size(); i++) {
			if (i)
				std::cout << ", ";
			std::cout << v[i];
		}
		std::cout << " }";
		return os;
	}

}; // class vector

} // namespace ft

#endif // VECTOR_HPP
