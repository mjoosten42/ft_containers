#ifndef META_HPP
#define META_HPP

#include <type_traits>

namespace ft
{

//	enable_if

template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> { typedef T type; };

//	is_pointer

template <typename T>
struct	is_pointer: std::false_type {};

template <typename T>
struct	is_pointer<T*>: std::true_type {};

template <typename T>
struct	is_pointer<T* const>: std::true_type {};

template <typename T>
struct	is_pointer<T* volatile>: std::true_type {};

template <typename T>
struct	is_pointer<T* const volatile>: std::true_type {};

} // namespace ft

#endif // META_HPP
