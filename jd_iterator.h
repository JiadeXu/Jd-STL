/*************************************************************************
	> File Name: jd_iterator.h
	> Author: 
	> Mail: 
	> Created Time: Thu Sep  9 11:30:35 2021
 ************************************************************************/

#ifndef _JD_ITERATOR_H
#define _JD_ITERATOR_H

#include "jd_macro.h"
#include <cstddef>
#include <iostream>

JD_SPACE_BEGIN

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag       : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<typename Category, 
	typename T,
	typename Distance = ptrdiff_t,
	typename Pointer = T*,
	typename Reference = T&>
class iterator {
	typedef Category 	iterator_category;
	typedef T 			value_type;
	typedef Distance 	difference_type;
	typedef Pointer 	pointer;
	typedef Reference 	reference;
};

template<class T>
struct iterator_traits {
    typedef typename T::difference_type   difference_type;
    typedef typename T::value_type        value_type;
    typedef typename T::pointer           pointer;
    typedef typename T::reference         reference;
    typedef typename T::iterator_category iterator_category;
};
template<class T>
struct iterator_traits<T *> {
	typedef T 							value_type;
    typedef T* 							pointer;
    typedef T& 							reference;
	typedef random_access_iterator_tag 	iterator_category;
	typedef ptrdiff_t 					difference_type;
};
template<class T>
struct iterator_traits<const T *> {
	typedef T 							value_type;
    typedef T& 							reference;
    typedef T* 							pointer;
	typedef random_access_iterator_tag 	iterator_category;
	typedef ptrdiff_t 					difference_type;
};

template<class T>
struct iterator_traits<T []> {
};

template<class T>
struct iterator_traits<const T []> {
};

// 获取迭代器类型
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&) {
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}

// 获取迭代器 distance type
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&) {
	return static_cast<typename iterator_traits<Iterator>::difference_type *>(0);
}

// 获取迭代器value_type
template<class Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator &) {
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 整组distance函数
template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag) {
	typename iterator_traits<InputIterator>::difference_type n = 0;
	while(first != last) {
		++n;
		++first;
	}
	return n;
}

template<class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
	return last - first;
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {
	return __distance(first, last, iterator_category(last));
}

// 整组advance
template<class InputIterator, class DistanceType>
inline void __advance(InputIterator &i, DistanceType n, input_iterator_tag) {
	while(n--) {
		++i;
	}
}

template<class BidirectionalIterator, class DistanceType>
inline void __advance(BidirectionalIterator &i, DistanceType n, bidirectional_iterator_tag) {
	if (n > 0) {
		while(n--) {
			++i;
		}
	} else {
		while(n++) {
			--i;
		}
	}
}

template<class RandomAccessIterator, class DistanceType>
inline void __advance(RandomAccessIterator &i, DistanceType n, random_access_iterator_tag) {
	i += n;
}

template<class InputIterator, class DistanceType>
inline void advance(InputIterator &i, DistanceType n) {
	__advance(i, n, iterator_category(i));
}

JD_SPACE_END

#endif
