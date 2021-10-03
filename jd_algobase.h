/*************************************************************************
	> File Name: jd_algobase.h
	> Author: 
	> Mail: 
	> Created Time: Sun Sep 12 10:21:44 2021
 ************************************************************************/

#ifndef _JD_ALGOBASE_H
#define _JD_ALGOBASE_H

#include "jd_macro.h"
#include "jd_iterator.h"
#include <algorithm>
#include <cstring>
#include <utility>

JD_SPACE_BEGIN

template<class T>
inline const T& max(const T &a, const T &b) {
	return a < b ? b : a;
}

template<class T, class Compare>
inline const T& max(const T &a, const T &b, Compare comp) {
	return comp(a, b) ? b : a;
}

template<class T>
inline const T& min(const T &a, const T &b) {
	return a < b ? a : b;
}

template<class T, class Compare>
inline const T& min(const T &a, const T &b, Compare comp) {
	return comp(a, b) ? a : b;
}


template<class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) {
		if (*first1 != *first2) {
			return false;
		}
	}
	return true;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!binary_pred(*first1, *first2)) {
			return false;
		}
	}
	return true;
}


template<class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 != *first2) {
			return false;
		}
	}
	return first1 == last1 && first2 == last2;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryPredicate binary_pred) {
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (!binary_pred(*first1, *first2)) {
			return false;
		}
	}
	return first1 == last1 && first2 == last2;
}

// fill
template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T &value) {
	for(; first != last; ++ first) {
		*first = value;
	}
}

template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T &x) {
	OutputIterator cur = first;
	while(n--) {
		*(cur++) = x;
	}
	return first;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
	typename JD::iterator_traits<ForwardIterator1>::value_type tmp = *a;
	*a = *b;
	*b = tmp;
}

// 以字典排列方式对两个序列 [first, last1) 和 [first2, last2) 进行比较
template<class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	// 任何一个序列到达尾端就结束
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (*first1 < *first2) {
			return true;
		}
		if (*first2 < *first1) {
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}

template<class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	// 任何一个序列到达尾端就结束
	for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
		if (comp(*first1, *first2)) {
			return true;
		}
		if (comp(*first2, *first1)) {
			return false;
		}
	}
	return first1 == last1 && first2 != last2;
}

inline bool lexicographical_compare(
	const unsigned char *first1,
	const unsigned char *last1,
	const unsigned char *first2,
	const unsigned char *last2) 
{
	const size_t len1 = last1 - first1;
	const size_t len2 = last2 - first2;

	const int result = memcmp(first1, first2, JD::min(len1, len2));
	return result != 0 ? result < 0 : len1 < len2;
}

template<class InputIteartor1, class InputIteartor2>
std::pair<InputIteartor1, InputIteartor2> mismatch(InputIteartor1 first1, InputIteartor1 last1, InputIteartor2 first2) {
	while (first1 != last1 && *first1 == *first2) {
		++first1;
		++first2;
	}
	return std::pair<InputIteartor1, InputIteartor2>(first1, first2);
}

template<class InputIteartor1, class InputIteartor2, class BinaryPredicate>
std::pair<InputIteartor1, InputIteartor2> mismatch(InputIteartor1 first1, InputIteartor1 last1, InputIteartor2 first2, BinaryPredicate binary_pred) {
	while (first1 != last1 && binary_pred(*first1, *first2)) {
		++first1;
		++first2;
	}
	return std::pair<InputIteartor1, InputIteartor2>(first1, first2);
}

template<class InputIterator, class _Function>
_Function for_each(InputIterator first, InputIterator last, _Function func) {
	for (; first != last; ++first) {
		func(*first);
	}
	return func;
}

template<class T>
inline void swap(T &a, T &b) {
	T tmp = a;
	a = b;
	b = tmp;
}

JD_SPACE_END

#endif
