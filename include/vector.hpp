#ifndef VECTOR_HPP
#define VECTOR_HPP

// https://github.com/electronicarts/EASTL/blob/master/include/EASTL/vector.h

#include <memory> // std::allocator

namespace ft 
{

	template <typename T, class Allocator = std::allocator<T> >
	class vector {
		public:
			typedef T*	iterator;
		
			vector(): _begin(NULL), _end(NULL), _capacity(0) {}
			//vector( const vector& rhs );
			~vector() {
				delete _begin;
			};
			//vector&	operator=( const vector& rhs );
		
			iterator	begin() const {
				return _begin;
			}
			iterator	end() const {
				return _end;
			}
			size_t	capacity() const {
				return _capacity;
			}
			size_t	size() const {
				return _end - _begin;
			}

			void	push_back(const T& value) {
				if (size() == _capacity) {
					_capacity = _capacity ? _capacity * 2 : 1;
					_begin = Allocator::allocate(_capacity);
				}
				*_end = value;
				_end++;
			}
			
		protected:
			T*		_begin;
			T*		_end;
			size_t	_capacity;
	};

} // namespace ft

#endif // VECTOR_HPP
