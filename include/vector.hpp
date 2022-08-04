#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // std::allocator
#include <cstddef> // std::size_t, std::ptrdiff_t
#include "meta.hpp" // enable_if, is_pointer // TODO
#include "iterator.hpp"

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
		typedef	reverseIterator<T>					reverse_iterator;
		typedef reverseIterator<const T>			const_reverse_iterator;

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
		typename enable_if<std::__is_input_iterator<InputIt>::value, bool>::type = true)
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
			if (_data) {
				clear();
				_allocator.deallocate(_data, capacity());
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
		typename enable_if<std::__is_input_iterator<InputIt>::value, bool>::type = true) {
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
		iterator			end() const { return begin() + size(); }
		reverse_iterator	rbegin() const { return reverse_iterator(end()); }
		reverse_iterator	rend() const { return reverse_iterator(begin()); }

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
			difference_type	index = pos - begin(); // Save index in case of reallocation
		
			insert(pos, 1, value);
			return begin() + index;
		}
 
		void	insert(iterator pos, size_type n, const T& value) {		
			difference_type	index = pos - begin(); // Save index in case of reallocation
			
			shoveRight(pos, n);
			for (size_type i = 0; i < n; i++)
				_allocator.construct(begin() + index + i, value);
			_size += n;
		}

		// TODO: remove std:: (possibly)
		// TODO: ra/contigous iterator specialization
		template <typename InputIt>
		void	insert(iterator pos, InputIt first, InputIt last,
		typename enable_if<std::__is_input_iterator<InputIt>::value, bool>::type = true) {
			difference_type	index = pos - begin(); // Save index in case of reallocation
			vector<T>		tmp(first, last);

			shoveRight(pos, tmp.size());
			for (size_type i = 0; i < tmp.size(); i++)
				_allocator.construct(begin() + index + i, tmp[i]);
			_size += tmp.size();
		}

		iterator	erase(iterator pos) {
			return erase(pos, pos + 1);
		}
	
		iterator	erase(iterator first, iterator last) {
			std::move(last, end(), first);
			_size -= last - first;
			return first;
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
			std::swap(_allocator, other._allocator);
			std::swap(_capacity, other._capacity);
			std::swap(_size, other._size);
			std::swap(_data, other._data);
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

	friend std::ostream&	operator<<(std::ostream& os, const vector<T>& v) { // TODO: remove
		std::cout << "size: " << std::setw(2) << v.size();
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

// Comparison operators

template <typename T, class Allocator>
bool	operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	if (lhs.size() != rhs.size())
		return false;
	return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, class Allocator>
bool	operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return !(lhs == rhs);
}

template <typename T, class Allocator>
bool	operator<(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, class Allocator>
bool	operator<=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return lhs < rhs || lhs == rhs;
}

template <typename T, class Allocator>
bool	operator>(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return rhs < lhs;
}

template <typename T, class Allocator>
bool	operator>=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return rhs <= lhs;
}

// swap
template <typename T, class Allocator>
void	swap(vector<T, Allocator>& lhs, vector<T, Allocator>& rhs) { lhs.swap(rhs); }

} // namespace ft

#endif // VECTOR_HPP
