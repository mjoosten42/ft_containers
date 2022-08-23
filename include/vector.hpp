#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // std::allocator
#include <cstddef> // std::size_t, std::ptrdiff_t
#include "meta.hpp" // enable_if
#include "iterator.hpp"

namespace ft 
{

template <typename T, class Allocator = std::allocator<T> >
class vector {

	public:
	
		//	Typedefs
	
		typedef T							value_type;
		typedef Allocator					allocator_type;
		typedef	std::size_t					size_type;
		typedef	std::ptrdiff_t				difference_type;
		typedef	T&							reference;
		typedef	const T&					const_reference;
		typedef	T*							pointer;
		typedef	const T*					const_pointer;
		typedef T*							iterator;
		typedef	const T*					const_iterator;
		typedef	reverseIterator<T*>			reverse_iterator;
		typedef reverseIterator<const T*>	const_reverse_iterator;

		//	Constructors
	
		vector(): _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const Allocator& alloc): _allocator(alloc), _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const vector& rhs): _capacity(0), _size(0), _data(NULL) { *this = rhs; }

		//	Initialize with n values
		explicit	vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
			: _allocator(alloc), _capacity(n), _size(0), _data(_allocator.allocate(n)) {
			std::cout << "C3\n";
			assign(n, value);
		}

		//	Range constructor
		template <typename InputIt, typename IC = typename iterator_traits<InputIt>::iterator_category>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			: _allocator(alloc), _capacity(0), _size(0), _data(NULL) {
			std::cout << "C4\n";
			assign(first, last);
		}

		//	Destructor
		~vector() {
			clear();
			_allocator.deallocate(_data, _capacity);
		};
	
		// Copy assignment
		vector&	operator=(const vector& rhs) {
			if (this == &rhs)
				return *this;
			if (_data) {
				clear();
				_allocator.deallocate(_data, capacity());
			}
			_allocator = rhs.get_allocator();
			_data = _allocator.allocate(rhs.capacity());
			assign(rhs.begin(), rhs.end());
			return *this;
		}

		void	assign(size_type n, const T& value) {
			clear();
			reserve(n);
			std::uninitialized_fill_n(begin(), n, value);
		}

		// TODO: ra/contigous iterator specialization;
		// Use template specialization with InputIt::iterator_category
		template <typename InputIt>
		void	assign(InputIt first, InputIt last) {
			clear();
			for (; first != last; first++) {
				push_back(*first);
			}
		}
	
		allocator_type	get_allocator() const { return _allocator; }

		//	Element access
	
		reference	at(size_type index) {
			if (index >= size())
				throw std::out_of_range("vector");
			return _data[index];
		}
	
		reference	operator[](size_type index) {
			return _data[index];
		}
	
		const_reference	operator[](size_type index) const {
			return _data[index];
		}
				
		reference	front() { return *begin(); }
		reference	back() { return *(end() - 1); }

		T*	data() { return _data; }
	
		//	Iterators

		iterator			begin() const {	return _data; }
		iterator			end() const { return begin() + size(); }
		reverse_iterator	rbegin() const { return end(); }
		reverse_iterator	rend() const { return begin(); }

		//	Capacity

		bool		empty() const { return size() == 0; }
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
			shoveRight(pos, n);
			std::uninitialized_fill_n(pos, n, value);
			_size += n;
		}

		// TODO: remove std:: (possibly)
		// TODO: ra/contigous iterator specialization
		template <typename InputIt>
		void	insert(iterator pos, InputIt first, InputIt last) {
			vector<T>	tmp(first, last);

			shoveRight(pos, tmp.size());
			std::uninitialized_copy(tmp.begin(), tmp.end(), pos);
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
			if (size() < n)
				insert(end(), n - size(), value);
			if (size() > n)
				erase(begin() + n, end());
		}

		void	swap(vector& other) {
			std::swap(_allocator, other._allocator);
			std::swap(_capacity, other._capacity);
			std::swap(_size, other._size);
			std::swap(_data, other._data);
		}
		
		// Comparison operators

		bool	operator==(const vector& rhs) const { return size() == rhs.size() && std::equal(begin(), end(), rhs.begin()); } // TODO: std --> ft
		bool	operator!=(const vector& rhs) const { return !(*this == rhs); }
		bool	operator< (const vector& rhs) const { return std::lexicographical_compare(begin(), end(), rhs.begin(), rhs.end()); } // TODO: std --> ft
		bool	operator<=(const vector& rhs) const { return *this < rhs || *this == rhs;	}
		bool	operator> (const vector& rhs) const { return rhs < *this; }
		bool	operator>=(const vector& rhs) const { return rhs <= *this; }

	private:

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
			size_type	i = end() - pos;
		
			reserve(size() + n);
			while (i--) {
				_allocator.construct(pos + n + i, *(pos + i));
				_allocator.destroy(pos + i);
			}
		}

	protected:

		Allocator	_allocator;
		size_type	_capacity;
		size_type	_size;
		pointer		_data;

}; // class vector

} // namespace ft

namespace std {
	template <typename T, typename Allocator>
	void	swap(ft::vector<T, Allocator>& lhs, ft::vector<T, Allocator>& rhs) { lhs.swap(rhs); }
}

#endif // VECTOR_HPP
