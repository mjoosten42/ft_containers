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
		explicit	vector(const vector& rhs)
				: _allocator(rhs.get_allocator()), _capacity(rhs.size()), _size(rhs.size()), _data(_allocator.allocate(rhs.size())) {
			try	{
				std::uninitialized_copy(rhs.begin(), rhs.end(), begin());	
			}
			catch(...) {
				_allocator.deallocate(_data, capacity());
				throw;
			}
		}

		explicit	vector(size_type n, const T& value = T(), const Allocator& alloc = Allocator())
				: _allocator(alloc), _capacity(n), _size(n), _data(_allocator.allocate(n)) {
			try	{
				std::uninitialized_fill_n(begin(), n, value);
			}
			catch(...) {
				_allocator.deallocate(_data, capacity());
				throw;
			}
		}

		template <typename InputIt>
		vector(InputIt first, InputIt last, const Allocator& alloc = Allocator(),
			typename ft::enable_if<!ft::is_integral<InputIt>::value, bool>::type = true)
				: _allocator(alloc), _capacity(0), _size(0), _data(NULL) {
			try	{
				insert(begin(), first, last);
			}
			catch(...) {
				_allocator.deallocate(_data, capacity());
				throw;
			}
		}

		~vector() {
			clear();
			_allocator.deallocate(_data, capacity());
		};
	
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
			if (n > 2 * size())
				reserve(n);
			while (size() < n)
				push_back(value);
			while (size() > n)
				pop_back();
		}

		void	swap(vector& other) {
			std::swap(_allocator, other._allocator);
			std::swap(_capacity, other._capacity);
			std::swap(_size, other._size);
			std::swap(_data, other._data);
		}

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
	
		template <typename InputIt>
		void	doInsert(iterator pos, InputIt first, InputIt last, std::input_iterator_tag) {
			size_type				index = pos - begin();
			vector<T, Allocator>	tmp;

			for (; first != last; first++)
				tmp.push_back(*first);
			shoveRight(pos, tmp.size());
			std::uninitialized_copy(tmp.begin(), tmp.end(), begin() + index);
			_size += tmp.size();
		}

		template <typename ForwardIt>
		void	doInsert(iterator pos, ForwardIt first, ForwardIt last, std::forward_iterator_tag) {
			size_type	index = pos - begin();
			size_type	size = std::distance(first, last);
		
			shoveRight(pos, size);
			std::uninitialized_copy(first, last, begin() + index);
			_size += size;
		}
	
		template <typename RandomAccessIt>
		void	doInsert(iterator pos, RandomAccessIt first, RandomAccessIt last, std::random_access_iterator_tag) {
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

template <typename T, typename Allocator>
bool	operator==(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, typename Allocator>
bool	operator< (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T, typename Allocator>
bool	operator!=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return !(lhs == rhs);
}

template <typename T, typename Allocator>
bool	operator> (const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return   rhs <  lhs ;
}

template <typename T, typename Allocator>
bool	operator<=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return !(lhs >  rhs);
}

template <typename T, typename Allocator>
bool	operator>=(const vector<T, Allocator>& lhs, const vector<T, Allocator>& rhs) {
	return !(lhs <  rhs);
}

} // namespace ft

namespace std {
	template <typename T, typename Allocator>
	void	swap(ft::vector<T, Allocator>& lhs, ft::vector<T, Allocator>& rhs) { lhs.swap(rhs); }
}

#endif // VECTOR_HPP
