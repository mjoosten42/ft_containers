#ifndef META_HPP
#define META_HPP

#include <type_traits>

namespace ft
{

// enable_if

template <bool B, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> { typedef T type; };

// remove_cv

template <typename T> struct remove_cv						{ typedef T type; };
template <typename T> struct remove_cv<const T>				{ typedef T type; };
template <typename T> struct remove_cv<volatile T>			{ typedef T type; };
template <typename T> struct remove_cv<const volatile T>	{ typedef T type; };

// is_integral

template <typename T> struct is_integral_imp		: public std::false_type{};

template <> struct is_integral_imp<bool>			: public std::true_type{};
template <> struct is_integral_imp<char>			: public std::true_type{};

template <> struct is_integral_imp<signed char>		: public std::true_type{};
template <> struct is_integral_imp<signed short>	: public std::true_type{};
template <> struct is_integral_imp<signed int>		: public std::true_type{};
template <> struct is_integral_imp<signed long>		: public std::true_type{};

template <> struct is_integral_imp<unsigned char>	: public std::true_type{};
template <> struct is_integral_imp<unsigned short>	: public std::true_type{};
template <> struct is_integral_imp<unsigned int>	: public std::true_type{};
template <> struct is_integral_imp<unsigned long>	: public std::true_type{};

template <typename T> struct is_integral	: public is_integral_imp<T>{};

} // namespace ft

#endif // META_HPP
