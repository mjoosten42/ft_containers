#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <memory> // std::allocator, std::uninitialized_fill_n
#include <cstddef> // std::size_t, std::ptrdiff_t
#include <algorithm> // std::move
#include "iterator.hpp"
#include "meta.hpp"

namespace ft 
{

template <typename T, class Allocator = std::allocator<T> >
class vector {

	public:
	
		//	Typedefs
	
		typedef T								value_type;
		typedef Allocator						allocator_type;
		typedef	std::size_t						size_type;
		typedef	std::ptrdiff_t					difference_type;
		typedef	T&								reference;
		typedef	const T&						const_reference;
		typedef	T*								pointer;
		typedef	const T*						const_pointer;
		typedef T*								iterator;
		typedef	const T*						const_iterator;
		typedef	reverseIterator<iterator>		reverse_iterator;
		typedef reverseIterator<const_iterator>	const_reverse_iterator;

		//	Constructors
	
		vector(): _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const Allocator& alloc): _allocator(alloc), _capacity(0), _size(0), _data(NULL) {}
		explicit	vector(const vector& rhs): _capacity(0), _size(0), _data(NULL) { *this = rhs; }

		//	Initialize with n values
		explicit	vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
			: _allocator(alloc), _capacity(n), _size(n), _data(_allocator.allocate(n)) {
			std::uninitialized_fill_n(begin(), n, value);
		}

		//	Range constructor
		template <typename InputIt>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator(),
				typename ft::enable_if<!ft::is_integral<InputIt>::value, bool>::type = true)
			: _allocator(alloc), _capacity(0), _size(0), _data(NULL) {
			insert(begin(), first, last);
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
			if (!empty()) {
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
			_size = n;
		}

		template <typename InputIt>
		typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type
		assign(InputIt first, InputIt last) {
			clear();
			insert(begin(), first, last);
		}
	
		allocator_type	get_allocator() const { return _allocator; }

		//	Element access
	
		reference		at(size_type index) {
			if (index >= size())
				throw std::out_of_range("vector");
			return _data[index];
		}
	
		const_reference	at(size_type index) const {
			if (index >= size())
				throw std::out_of_range("vector");
			return _data[index];
		}
	
		reference		operator[](size_type index) {
			return _data[index];
		}
	
		const_reference	operator[](size_type index) const {
			return _data[index];
		}
				
		reference		front() { return *begin(); }
		const_reference	front() const { return *begin(); }
		reference		back() { return *(end() - 1); }
		const_reference	back() const { return *(end() - 1); }

		pointer			data() { return _data; }
		const_pointer	data() const { return _data; }
	
		//	Iterators

		iterator		begin() { return data(); }
		iterator		end() { return begin() + size(); }
		const_iterator	begin() const {	return data(); }
		const_iterator	end() const { return begin() + size(); }

		reverse_iterator		rbegin() { return reverse_iterator(end()); }
		reverse_iterator		rend() { return reverse_iterator(begin()); }
		const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }
		const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

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
			erase(begin(), end());
		}

		iterator	insert(iterator pos, const T& value) {
			size_type	index = pos - begin();
		
			insert(pos, 1, value);
			return begin() + index;
		}
 
		void	insert(iterator pos, size_type n, const T& value) {
			size_type	index = pos - begin();

			shoveRight(pos, n);
			std::uninitialized_fill_n(begin() + index, n, value);
			_size += n;
		}

		// Chooses an insert depending on the iterator tag
		template <typename InputIt>
		typename ft::enable_if<!ft::is_integral<InputIt>::value, void>::type
		insert(iterator pos, InputIt first, InputIt last) {
			typedef typename iterator_traits<InputIt>::iterator_category IC;
			doInsert(pos, first, last, IC());
		}

		iterator	erase(iterator pos) {
			return erase(pos, pos + 1);
		}
	
		iterator	erase(iterator first, iterator last) {
			size_type	size = last - first;
		
			std::move(last, end(), first);
			while (size--)
				pop_back();
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
 		
		friend bool	operator==(const vector& lhs, const vector& rhs) const {
			return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
		}
	
		friend bool	operator< (const vector& lhs, const vector& rhs) const {
			return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
		}
	
		friend bool	operator!=(const vector& lhs, const vector& rhs) const { return !(lhs == rhs); }
		friend bool	operator<=(const vector& lhs, const vector& rhs) const { return lhs < rhs || lhs == rhs;	}
		friend bool	operator> (const vector& lhs, const vector& rhs) const { return rhs < lhs; }
		friend bool	operator>=(const vector& lhs, const vector& rhs) const { return rhs <= lhs; }

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
			size_type	index = pos - begin();
			size_type	i = end() - pos;
		
			reserve(size() + n);
			pos = begin() + index;
			while (i--) {
				_allocator.construct(pos + n + i, *(pos + i));
				_allocator.destroy(pos + i);
			}
		}
	
		// InputIt
		template <typename InputIt>
		void	doInsert(iterator pos, InputIt first, InputIt last, std::input_iterator_tag) {
			size_type	index = pos - begin();
			vector<T>	tmp;

			for (; first != last; first++)
				tmp.push_back(*first);
			shoveRight(pos, tmp.size());
			std::uninitialized_copy(tmp.begin(), tmp.end(), begin() + index);
			_size += tmp.size();
		}

		// ForwardIt && BiDirectionalIt
		template <typename InputIt>
		void	doInsert(iterator pos, InputIt first, InputIt last, std::forward_iterator_tag) {
			size_type	index = pos - begin();
			size_type	size = std::distance(first, last);
		
			shoveRight(pos, size);
			std::uninitialized_copy(first, last, begin() + index);
			_size += size;
		}
	
		// RandomAccessIt && ContigiousIt
		template <typename InputIt>
		void	doInsert(iterator pos, InputIt first, InputIt last, std::random_access_iterator_tag) {
			size_type	index = pos - begin();
			size_type	size = last - first;
		
			shoveRight(pos, size);
			std::uninitialized_copy(first, last, begin() + index);
			_size += size;
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
