#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // std::allocator
#include <cstddef> // std::size_t, std::ptrdiff_t
#include "meta.hpp" // enable_if, is_pointer // TODO

namespace ft 
{

template <typename T, class Allocator = std::allocator<T> >
class vector {

	public:
	
		//	Typedefs
	
		typedef T									value_type;
		typedef Allocator							allocator_type;
		typedef	std::size_t							size_type;
		typedef	std::ptrdiff_t						difference_type;
		typedef	T&									reference;
		typedef	const T&							const_reference;
		typedef	typename Allocator::pointer			pointer;
		typedef	typename Allocator::const_pointer	const_pointer;
		typedef T*									iterator;
		typedef	const T*							const_iterator;
		typedef	T*									reverse_iterator; // TODO
		typedef	const T*							const_reverse_iterator; // TODO

		//	Constructors
	
		vector(): _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const Allocator& alloc): _allocator(alloc), _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const vector& rhs): _capacity(0), _size(0), _data(NULL) { *this = rhs; }

		//	Initialize with n values
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

		//	Destructor
		~vector() {
			clear();
			_allocator.deallocate(_data, _capacity);
		};
	
		// Copy assignment
		vector&	operator=( const vector& rhs ) {
			if (this == &rhs)
				return *this;
			if (_capacity < rhs.size()) {
				clear();
				_allocator.deallocate(_data, _capacity);
				_data = rhs.get_allocator().allocate(rhs.capacity());
				_capacity = rhs.capacity();
			}
			_allocator = rhs.get_allocator();
			assign(rhs.begin(), rhs.end());
			return *this;
		}

		//	Objects beyond size are not constructed, but std::fill would call their destructor because of assignment
		void	assign(size_type n, const T& value) {
			reserve(n);
			while (size() > n)
				pop_back();
			std::fill_n(begin(), size(), value);
			while (size() < n)
				push_back(value);
		}

		// TODO: ra/contigous iterator specialation
		template <typename InputIt>
		void	assign(InputIt first, InputIt last,
		typename enable_if<std::is_pointer<InputIt>::value, bool>::type = true) {
			clear();
			for (; first != last; first++) {
				push_back(*first);
			}
		}
	
		allocator_type	get_allocator() const { return _allocator; }

		//	Element access
	
		reference	at(size_type index) const {
			if (index >= size())
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

		iterator			begin() const {	return _data; }
		iterator			end() const { return begin() + _size; }
		reverse_iterator	rbegin() const { return end() - 1; } // TODO
		reverse_iterator	rend() const { return begin() - 1; } // TODO

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
			while (size())
				pop_back();
		}

		iterator	insert(iterator pos, const T& value) {
			difference_type	index = pos - begin();
		
			insert(pos, 1, value);
			return begin() + index;
		}
 
		void	insert(iterator pos, size_type n, const T& value) {		
			difference_type	index = pos - begin();
			
			shoveRight(pos, n);
			for (size_type i = 0; i < n; i++)
				_allocator.construct(begin() + index + i, value);
			_size += n;
		}

		// TODO: ra/contigous iterator specialization
		template <typename InputIt>
		void	insert(iterator pos, InputIt first, InputIt last,
		typename enable_if<std::is_pointer<InputIt>::value, bool>::type = true) {
			difference_type	index = pos - begin();
		
			for (; first != last; first++)
				insert(begin() + index++, *first);
		}

		iterator	erase(iterator pos) {
			return erase(pos, pos + 1);
		}
	
		iterator	erase(iterator first, iterator last) {
			std::move(last, end(), first);
			_size -= last - first;
			return last;
		}
	
		void	push_back(const T& value) {
			if (_size == _capacity)
				setCapacity(_capacity ? _capacity * 2 : 1);
			_allocator.construct(end(), value);
			_size++;
		}

		void	pop_back() {
			_allocator.destroy(end() - 1);
			_size--;
		}

		void	resize(size_type n, T value = T()) {
			reserve(n);
			while (size() > n)
				pop_back();
			if (size() < n)
				insert(end(), n - size(), value);
		}

		void	swap(vector& other) {
			vector<T>	tmp;

			tmp = *this;
			*this = other;
			other = tmp;
			// std::swap(_allocator, other._allocator);
			// std::swap(_capacity, other._capacity);
			// std::swap(_size, other._size);
			// std::swap(_data, other._data);
		}
		
	protected:

		void	setCapacity(size_type newCapacity) {
			pointer	tmp = _allocator.allocate(newCapacity);

			for (size_type i = 0; i < size(); i++) {
				_allocator.construct(tmp + i, *(begin() + i));
				_allocator.destroy(begin() + i);
			}
			_allocator.deallocate(_data, _capacity);
			_capacity = newCapacity;
			_data = tmp;
		}

		void	shoveRight(iterator pos, size_type n) {
			difference_type	index = pos - begin(); // Save index in case of reallocation

			reserve(size() + n);
			difference_type i = size() - index;
			while (i--) {
				_allocator.construct(begin() + index + n + i, *(begin() + index + i));
				_allocator.destroy(begin() + index + i);
			}
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
