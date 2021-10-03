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
#include "jd_type_traits.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <ostream>
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

// copy
template<class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, JD::input_iterator_tag) {
	J_LOG();
	// 以迭代器等同与否，决定循环是否继续 速度较慢
	for (; first != last; ++first) {
		*result = *first;
	}
	return result;
}

template<class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
	// 以n决定循环的执行次数，速度快
	for (Distance n = last - first; n > 0; --n, ++result, ++first) {
		*result = *first;
	}
	J_LOG();
	return result;
}

template<class RandomAccessIterator, class OutputIterator>
inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, JD::random_access_iterator_tag) {
	// 以迭代器等同与否，决定循环是否继续 速度较慢
	J_LOG();
	return __copy_d(first, last, result, distance_type(first));
}

template<class InputIterator, class OutputIterator>
struct __copy_dispatch {
	OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
		// typedef typename JD::iterator_traits<InputIterator>::iterator_category
		J_LOG();
		return __copy(first, last, result, JD::iterator_category(first));
	}
};

template<class T>
inline T* __copy_t(const T *first, const T *last, T *result, JD::__true_type) {
	J_LOG();
	memmove(result, first, sizeof(T) * (last - first));
	return result + (last - first);
}

template<class T>
inline T* __copy_t(const T *first, const T *last, T *result, JD::__false_type) {
	J_LOG();
	return __copy_d(first, last, result, (ptrdiff_t *) 0);
}

template<class T>
struct __copy_dispatch<T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		J_LOG();
		typedef typename JD::__type_traits<T>::has_tirvial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template<class T>
struct __copy_dispatch<const T*, T*> {
	T* operator()(T* first, T* last, T* result) {
		J_LOG();
		typedef typename JD::__type_traits<T>::has_tirvial_assignment_operator t;
		return __copy_t(first, last, result, t());
	}
};

template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
	J_LOG();
	return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
}

inline char *copy(const char *first, const char *last, char *result) {
	J_LOG();
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t *copy(const wchar_t *first, const wchar_t *last, wchar_t *result) {
	J_LOG();
	memmove(result, first, (last - first) * sizeof(wchar_t));
	return result + (last - first);
}

template<class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, JD::bidirectional_iterator_tag, Distance*) {
	J_LOG();
	while(first != last) {
		*(--result) = *(--last);
	}
	return result;
}

template<class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, JD::random_access_iterator_tag, Distance*) {
	J_LOG();
	for(Distance n = last - first; n > 0; --n) {
		*(--result) = *(--last);
	}
	return result;
}

#ifndef JD_NOT_PARTIAL_SPECIALIZATION

template<class BidirectionalIterator1, class BidirectionalIterator2, class BoolType>
struct __copy_backward_dispatch {
	typedef typename JD::iterator_traits<BidirectionalIterator1>::iterator_category Cat;
	typedef typename JD::iterator_traits<BidirectionalIterator1>::difference_type Distance;

	static BidirectionalIterator2 copy(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
		J_LOG();
		return __copy_backward(first, last, result, Cat(), (Distance *) 0);
	}
};

template<class T>
struct __copy_backward_dispatch<T*, T*, JD::__true_type> {
	static T* copy(T* first, T* last, T* result) {
		J_LOG();
		const ptrdiff_t n = last - first;
		memmove(result - n, first, n * sizeof(T));
		return result - n;
	}
};

template<class T>
struct __copy_backward_dispatch<const T*, T*, JD::__true_type> {
	static T* copy(T* first, T* last, T* result) {
		J_LOG();
		const ptrdiff_t n = last - first;
		memmove(result - n, first, n * sizeof(T));
		return result - n;
	}
};

template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
	typedef typename JD::__type_traits<typename JD::iterator_traits<BidirectionalIterator1>::value_type>::has_tirvial_assignment_operator is_trivial;
	J_LOG();
	return __copy_backward_dispatch<BidirectionalIterator1, BidirectionalIterator2, is_trivial>::copy(first, last, result);
}

#else
// 不适用类泛型特化
template<class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
	J_LOG();
	return JD::__copy_backward(first, last, result, JD::iterator_category(first), JD::distance_type(first));
}

#endif
JD_SPACE_END

#endif
