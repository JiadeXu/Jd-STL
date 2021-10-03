/*************************************************************************
	> File Name: jd_uninitialized.h
	> Author: 
	> Mail: 
	> Created Time: Sun Sep 12 15:57:39 2021
 ************************************************************************/

#ifndef _JD_UNINITIALIZED_H
#define _JD_UNINITIALIZED_H

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <ostream>
#include "jd_type_traits.h"
#include "jd_algobase.h"
#include "jd_macro.h"
#include "jd_construct.h"

JD_SPACE_BEGIN

// __uninitialized_fill_n
template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __false_type) {
	ForwardIterator cur = first;
	for(; n > 0; --n, ++cur) {
		construct(&*cur, x);
	}
	return cur;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T &x, __true_type) {
	return JD::fill_n(first, n, x); // 6.4.2节
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first, Size n, const T &x, T1 *) {
	typedef typename __type_traits<T1>::is_POD_type is_PDO;
	return __uninitialized_fill_n_aux(first, n, x, is_PDO());
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T &x) {
	return __uninitialized_fill_n(first, n, x, value_type(first));
}

// __uninitialized_fill
template<class ForwardIterator, class T>
inline ForwardIterator __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __false_type) {
	ForwardIterator cur = first;
	for(; cur != last; ++cur) {
		// 必须一个一个元素的构造
		construct(&*cur, x);
	}
	return cur;
}

template<class ForwardIterator, class T>
inline ForwardIterator __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T &x, __true_type) {
	std::fill(first, last, x); // std
	return first;
}

template<class ForwardIterator, class T, class T1>
inline ForwardIterator __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x, T1 *) {
	typedef typename __type_traits<T1>::is_POD_type is_PDO;
	return __uninitialized_fill_aux(first, last, x, is_PDO());
}

template<class ForwardIterator, class T>
inline ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T &x) {
	return __uninitialized_fill(first, last, x, value_type(first));
}

// copy
// 不是 Plain Old Data
template<class InputIteartor, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIteartor first, InputIteartor last, ForwardIterator result, __false_type) {
	ForwardIterator cur = result;
	for (; first != last; ++first, ++cur) {
		construct(&*cur, *first);
	}
	return cur;
}

// 是 Palin Old Data 类型
template<class InputIteartor, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIteartor first, InputIteartor last, ForwardIterator result, __true_type) {
	return JD::copy(first, last, result); // 调用STL copy
}

template<class InputIteartor, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIteartor first, InputIteartor last, ForwardIterator result, T*) {
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIteartor, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIteartor first, InputIteartor last, ForwardIterator result) {
	return __uninitialized_copy(first, last, result, value_type(result));
}
// 针对 char* 和 wchar_t
inline char* uninitialized_copy(const char *first, const char *last, char *result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* uninitialized_copy(const wchar_t *first, const wchar_t *last, wchar_t *result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

JD_SPACE_END

#endif
