#ifndef META_HPP
#define META_HPP

#include <type_traits>

//	enable_if

template <bool B, typename T>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
	typedef T type;
};

//	is_pointer

template <typename T>
struct	is_pointer: std::false_type {};

template <typename T>
struct	is_pointer<T*>: std::true_type {};

template <typename T>
struct	is_pointer<T* const>: std::true_type {};

#endif // META_HPP
