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

// set_union
template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	J_LOG();
	while (first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
		} else if (*first2 < *first1) {
			*result = *first2;
			++first2;
		} else {
			*result = *first1;
			++first1;
			++first2;
		}
		++result;
	}
	// 拼接上剩余部分
	return JD::copy(first2, last2, JD::copy(first1, last1, result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	J_LOG();
	while(first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			++first1;
		} else if (*first2 < *first1) {
			++first2;
		} else {
			*result = *first1;
			++first1;
			++first2;
			++result;
		}
	}
	return result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	J_LOG();
	while(first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		} else if (*first2 < *first1) {
			++first2;
		} else {
			++first1;
			++first2;
		}
	}
	return JD::copy(first1, last1, result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	J_LOG();
	while(first1 != last1 && first2 != last2) {
		if (*first1 < *first2) {
			*result = *first1;
			++first1;
			++result;
		} else if (*first2 < *first1) {
			*result = *first2;
			++first2;
			++result;
		} else {
			++first2;
			++first1;
		}
	}
	return JD::copy(first2, last2, JD::copy(first1, last1, result));
}

template<class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) return first;
		first = next;
	}
	return last;
}

template<class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last) return last;
	ForwardIterator next = first;
	while (++next != last) {
		if (binary_pred(*first, *next)) return first;
		first = next;
	}
	return last;
}

template<class InputIterator, class T>
typename JD::iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T &value) {
	typename JD::iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first) {
		if (*first == value) {
			++n;
		}
	}
	return n;
}

template<class InputIterator, class Predicate>
typename JD::iterator_traits<InputIterator>::difference_type count_if(InputIterator first, InputIterator last, Predicate pred) {
	typename JD::iterator_traits<InputIterator>::difference_type n = 0;
	for (; first != last; ++first) {
		if (pred(*first)) {
			++n;
		}
	}
	return n;
}

template<class ForwardIteartor1, class ForwardIteartor2, class Distance1, class Distance2>
ForwardIteartor1 __search(ForwardIteartor1 first1, ForwardIteartor1 last1, ForwardIteartor2 first2, ForwardIteartor2 last2, Distance1 *, Distance2 *) {
	Distance1 d1 = JD::distance(first1, last1);
	Distance2 d2 = JD::distance(first2, last2);

	// std::cout << "d1 " << d1 << " d2 " << d2 << std::endl;

	ForwardIteartor1 cur1 = first1;
	ForwardIteartor2 cur2 = first2;

	while (cur2 != last2) {
		if (*cur1 == *cur2) {
			++cur1;
			++cur2;
		} else {
			if (d1 == d2) {
				return last1;
			} else {
				cur1 = ++first1;
				cur2 = first2;
				--d1;
			}
		}
	}
	return first1;
}

template<class ForwardIteartor1, class ForwardIteartor2>
inline ForwardIteartor1 search(ForwardIteartor1 first1, ForwardIteartor1 last1, ForwardIteartor2 first2, ForwardIteartor2 last2) {
	return JD::__search(first1, last1, first2, last2, JD::distance_type(first1), distance_type(first2));
}

template<class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T &value) {
	while (first != last && *first != value) {
		++first;
	}
	return first;
}

template<class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
	while (first != last && !pred(*first)) {
		++first;
	}
	return first;
}

template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, JD::forward_iterator_tag, JD::forward_iterator_tag) {
	if (first2 == last2) return last1;

	ForwardIterator1 result = last1;
	for(;;) {
		ForwardIterator1 new_result = JD::search(first1, last1, first2, last2);
		if (new_result == result) {
			return result;
		} else {
			result = new_result;
			first1 = new_result;
			++first1;
		}
	}
	return result;
}

// template<class ForwardIterator1, class ForwardIterator2>
// ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, JD::bidirectional_iterator_tag, JD::bidirectional_iterator_tag) {
// 	// if (first2 == last2) return last1;

// 	// ForwardIterator1 result = last1;
// 	// for(;;) {
// 	// 	ForwardIterator1 new_result = JD::search(first1, last1, first2, last2);
// 	// 	if (new_result == result) {
// 	// 		return result;
// 	// 	} else {
// 	// 		result = new_result;
// 	// 		first1 = new_result;
// 	// 		++first1;
// 	// 	}
// 	// }
// }

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	typename JD::iterator_traits<ForwardIterator1>::iterator_category cat1;
	typename JD::iterator_traits<ForwardIterator2>::iterator_category cat2;

	// return JD::__find_end(first1, last1, first2, last2, cat1(), cat2());
	if (first2 == last2) return last1;

	ForwardIterator1 result = last1;
	for(;;) {
		ForwardIterator1 new_result = JD::search(first1, last1, first2, last2);
		if (new_result == last1) {
			return result;
		} else {
			result = new_result;
			first1 = new_result;
			++first1;
		}
	}
	return result;
}

template<class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
	for (; first1 != last1; ++first1) {
		for (ForwardIterator iter = first2; iter != last2; ++iter) {
			if (*first1 == *iter) {
				return first1;
			}
		}
	}
	return last1;
}

template<class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate comp) {
	for (; first1 != last1; ++first1) {
		for (ForwardIterator iter = first2; iter != last2; ++iter) {
			if (comp(*first1, *iter)) {
				return first1;
			}
		}
	}
	return last1;
}

template<class ForwardIteartor, class Integer, class T>
inline ForwardIteartor search_n(ForwardIteartor first, ForwardIteartor last, Integer count, const T &value) {
	if (count <= 0) {
		return first;
	} else {
		first = JD::find(first, last, value);
		while (first != last) {
			Integer n = count - 1;
			ForwardIteartor i = first;
			++i;
			while (i != last && n != 0 && *i == value) {
				++i;
				--n;
			}
			if (n == 0) {
				return first;
			} else {
				first = JD::find(i, last, value);
			}
		}
		return last;
	}
}

template<class ForwardIteartor, class Integer, class T, class BinaryPredicate>
inline ForwardIteartor search_n(ForwardIteartor first, ForwardIteartor last, Integer count, const T &value, BinaryPredicate binary_pred) {
	if (count <= 0) {
		return first;
	} else {
		while (first != last) {
			if (binary_pred(*first, value)) break;
			++first;
		}

		while (first != last) {
			Integer n = count - 1;
			ForwardIteartor i = first;
			++i;
			while (i != last && n != 0 && binary_pred(*i, value)) {
				++i;
				--n;
			}
			if (n == 0) {
				return first;
			} else {
				while (i != last) {
					if (binary_pred(*i, value)) break;
					++i;
				}
				first = i;
			}
		}
		return last;
	}
}

template<class ForwardIterator, class Generate>
void generate(ForwardIterator first, ForwardIterator last, Generate gen) {
	for (; first != last; ++first) {
		*first = gen();
	}
}

template<class OutputIterator, class Size, class Generate>
OutputIterator generate_n(OutputIterator first, Size n, Generate gen) {
	for (; n > 0;  --n, ++first) {
		*first = gen();
	}
	return first;
}

template<class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			return false;
		} else if (*first1 < *first2) {
			++first1;
		} else {
			++first1;
			++first2;
		}
	}
	return first2 == last2;
}

template<class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2, *first1)) {
			return false;
		} else if (comp(*first1, *first2)) {
			++first1;
		} else {
			++first1;
			++first2;
		}
	}
	return first2 == last2;
}

template<class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
	if (first == last) return first;
	ForwardIterator result = first;
	while(++first != last) {
		if (*result < *first) result = first;
	}
	return result;
}

template<class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if (first == last) return first;
	ForwardIterator result = first;
	while(++first != last) {
		if (comp(*result, *first)) result = first;
	}
	return result;
}

template<class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
	if (first == last) return first;
	ForwardIterator result = first;
	while(++first != last) {
		if (*first < *result) result = first;
	}
	return result;
}

template<class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if (first == last) return first;
	ForwardIterator result = first;
	while(++first != last) {
		if (comp(*first, *result)) result = first;
	}
	return result;
}

template<class BidirectionalIter1, class BidirectionalIter2, class BidirectionalIter3>
BidirectionalIter3 __merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1, BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter3 result) {
	if (first1 == last1) return JD::copy_backward(first2, last2, result);
	if (first2 == last2) return JD::copy_backward(first1, last1, result);
	--last1;
	--last2;
	for(;;) {
		if (*last2 < *last1) {
			*--result = *last1;
			if (first1 == last1) {
				return JD::copy_backward(first2, ++last2, result);
			}
			--last1;
		} else {
			*--result = *last2;
			if (first2 == last2) {
				return JD::copy_backward(first1, ++last1, result);
			}
			--last2;
		}
	}
}

template<class BidirectionalIter1, class BidirectionalIter2, class BidirectionalIter3, class Compare>
BidirectionalIter3 __merge_backward(BidirectionalIter1 first1, BidirectionalIter1 last1, BidirectionalIter2 first2, BidirectionalIter2 last2, BidirectionalIter3 result, Compare comp) {
	if (first1 == last1) return JD::copy_backward(first2, last2, result);
	if (first2 == last2) return JD::copy_backward(first1, last1, result);
	--last1;
	--last2;
	for(;;) {
		if (comp(*last2, *last1)) {
			*--result = *last1;
			if (first1 == last1) {
				return JD::copy_backward(first2, ++last2, result);
			}
			--last1;
		} else {
			*--result = *last2;
			if (first2 == last2) {
				return JD::copy_backward(first1, ++last1, result);
			}
			--last2;
		}
	}
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while(first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			*result = *first2;
			++first2;
		} else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return JD::copy(first2, last2, JD::copy(first1, last1, result));
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
	while(first1 != last1 && first2 != last2) {
		if (comp(*first2, *first1)) {
			*result = *first2;
			++first2;
		} else {
			*result = *first1;
			++first1;
		}
		++result;
	}
	return JD::copy(first2, last2, JD::copy(first1, last1, result));
}

template<class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
	for (;;) {
		for(;;) {
			if (first == last) 
				return first;
			else if (pred(*first)) 
				 // 不符合移动条件
				 ++first;
			else
				break;
		}
		--last;
		for(;;) {
			if (first == last)
				return first;
			else if (!pred(*last))
				--last;
			else
			 	break;
		}
		JD::iter_swap(first, last);
		++first;
	}
}

template<class ForwardIterator, class OutputIterator, class T>
ForwardIterator remove_copy(ForwardIterator first, ForwardIterator last, OutputIterator result, const T& value) {
	for (; first != last; ++first) {
		if (*first != value) {
			*result = *first;
			++result;
		}
	}
	return result;
}

template<class ForwardIterator, class OutputIterator, class Predicate>
ForwardIterator remove_copy_if(ForwardIterator first, ForwardIterator last, OutputIterator result, Predicate pred) {
	for (; first != last; ++first) {
		if (!pred(*first)) {
			*result = *first;
			++result;
		}
	}
	return result;
}

template<class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value) {
	first = JD::find(first, last, value);
	ForwardIterator next = first;
	return  first == last ? first : JD::remove_copy(++next, last, first, value);
}

template<class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
	first = JD::find_if(first, last, pred);
	ForwardIterator next = first;
	return  first == last ? first : JD::remove_copy_if(++next, last, first, pred);
}

template<class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T &old_value, const T &new_value) {
	for (; first != last; ++first) {
		if (*first == old_value) {
			*first = new_value;
		}
	}
}

template<class ForwardIterator, class OutputIterator, class T>
void replace_copy(ForwardIterator first, ForwardIterator last, OutputIterator result, const T &old_value, const T &new_value) {
	for (; first != last; ++first, ++result) {
		*result = *first == old_value ? new_value : *first;
	}
}

template<class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T &new_value) {
	for (; first != last; ++first) {
		if (pred(*first)) {
			*first = new_value;
		}
	}
}

template<class ForwardIterator, class OutputIterator, class Predicate, class T>
void replace_copy_if(ForwardIterator first, ForwardIterator last, OutputIterator result, Predicate pred, const T &new_value) {
	for (; first != last; ++first, ++result) {
		*result = pred(*first) ? new_value : *first;
	}
}

template<class BidirectionalIterator>
inline void __reverse(BidirectionalIterator first, BidirectionalIterator last, JD::bidirectional_iterator_tag) {
	for(;;) {
		if (first == last || first == --last) {
			return;
		} else {
			JD::iter_swap(first++, last);
		}
	}
}

template<class RandomAccessIterator>
inline void __reverse(RandomAccessIterator first, RandomAccessIterator last, JD::random_access_iterator_tag) {
	while (first < last) {
		JD::iter_swap(first++, --last);
	}
}

template<class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
	JD::__reverse(first, last, JD::iterator_category(first));
}

template<class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
	while (first != last) {
		--last,
		*result = *last;
		++result;
	}
	return result;
}

template<class ForwardIterator, class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, Distance*, JD::forward_iterator_tag) {
	for (ForwardIterator i = middle;;) {
		JD::iter_swap(first, i);
		++first;
		++i;
		// 以下判断是前段[first, middle) 先结束还是 后段[middle, last) 先结束
		if (first == middle) {
			if (i == last) return;
			middle = i;
		} else if (i == last) {
			i = middle;
		}
	}
}

template<class BidirectionalIterator, class Distance>
void __rotate(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance*, JD::bidirectional_iterator_tag) {
	JD::reverse(first, middle);
	JD::reverse(middle, last);
	JD::reverse(first, last);
}

template<class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
	while (n != 0) {
		EuclideanRingElement t = m % n;
		m = n; 
		n = t;
	}
	return m;
}

template<class RandomAccessIterator, class Distance, class T>
void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T*) {
	T value = *initial;
	RandomAccessIterator ptr1 = initial;
	RandomAccessIterator ptr2 = ptr1 + shift;
	while (ptr2 != initial) {
		*ptr1 = *ptr2;
		ptr1 = ptr2;
		if (last - ptr2 > shift) {
			ptr2 += shift;
		} else {
			ptr2 = first + (shift - (last - ptr2));
		}
	}
	*ptr1 = value;
}

template<class RandomAccessIterator, class Distance>
void __rotate(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Distance*, JD::random_access_iterator_tag) {
	Distance n = __gcd(last - first, middle - first);
	while (n--) {
		JD::__rotate_cycle(first, last, first + n, middle - first, JD::value_type(first));
	}
}

template<class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last) {
	if (first == middle || middle == last) return;
	JD::__rotate(first, middle, last, JD::distance_type(first), JD::iterator_category(first));
}

template<class ForwardIterator, class OutputIterator>
inline OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result) {
	return JD::copy(first, middle, JD::copy(middle, last, result));
}

template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2) {
	for (; first1 != last1; ++first1, ++first2) {
		JD::iter_swap(first1, first2);
	}
	return first2;
}

template<class InputIterator, class OutputIterator, class UnaryOperation>
UnaryOperation transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op) {
	for (; first != last; ++first, ++result) {
		*result = op(*first);
	}
	return op;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOperation>
BinaryOperation transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, OutputIterator result, BinaryOperation binary_op) {
	for (; first1 != last1; ++first1, ++first2, ++result) {
		*result = binary_op(*first1, *first2);
	}
	return binary_op;
}

template<class InputIteartor, class OutputIterator>
inline OutputIterator __unique_copy(InputIteartor first, InputIteartor last, OutputIterator result, JD::output_iterator_tag) {
	typedef typename JD::iterator_traits<InputIteartor>::value_type v_t;
	v_t value = *first;
	*result = value;
	while (++first != last) {
		if (value != *first) {
			value = *first;
			*(++result) = value;
		}
	}
	return ++result;
}

template<class InputIteartor, class ForwardIterator>
ForwardIterator __unique_copy(InputIteartor first, InputIteartor last, ForwardIterator result, JD::forward_iterator_tag) {
	*result = *last;
	while(++first != last) {
		if (*result != *first) *(++result) = *first;
	}
	return result;
}

template<class ForwardIterator, class OutputIterator>
OutputIterator unique_copy(ForwardIterator first, ForwardIterator last, OutputIterator result) {
	return JD::__unique_copy(first, last, result, JD::iterator_category(result));
}

template<class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
	first = JD::adjacent_find(first, last);
	return JD::unique_copy(first, last, first);
}

JD_SPACE_END

#endif
