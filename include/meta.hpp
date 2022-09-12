#ifndef META_HPP
#define META_HPP

#include <type_traits> // std:true_type

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

template <typename T> struct is_integral: public is_integral_imp<typename remove_cv<T>::type>{};

// equal

template <typename InputIt1, typename InputIt2>
bool	equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
	for (;first1 != last1; ++first1, ++first2)
		if (!(*first1 == *first2))
			return false;
	return true;
}

// lexicographical_compare

template <typename InputIt1, typename InputIt2>
bool	lexicographical_compare(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2) {
	for (;first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2)
			return true;
		if (*first2 < *first1)
			return false;
	}
	return first1 == last1 && first2 != last2;
}

} // namespace ft

#endif // META_HPP
