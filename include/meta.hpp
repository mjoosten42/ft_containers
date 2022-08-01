#ifndef META_HPP
#define META_HPP

#include <type_traits>

namespace ft
{

// enable_if

template <bool B, typename T>
struct enable_if {};

template <typename T>
struct enable_if<true, T> { typedef T type; };

} // namespace ft

#endif // META_HPP
