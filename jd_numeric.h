/*************************************************************************
	> File Name: jd_numeric.h
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 15:30:41 2021
	++result ：表示返回的是输出区间的最尾端位置
 ************************************************************************/

#ifndef _JD_NUMERIC_H
#define _JD_NUMERIC_H

#include "jd_macro.h"
#include "jd_iterator.h"
#include <functional>

JD_SPACE_BEGIN

template<class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
	for (; first != last; ++first) {
		init = init + *first;
	}
	return init;
}

template<class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binay_op) {
	for (; first != last; ++first) {
		init = binay_op(init, *first); // 对每一个元素执行二元操作
	}
	return init;
}

// 计算一个序列中的相邻元素的差并存储到result目标序列中；链表或者数组；第一个元素会被保存
template<class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	typedef typename JD::iterator_traits<InputIterator>::value_type v_t;
	v_t value = *first;
	while(++first != last) {
		v_t tmp = *first;
		*(++result) = tmp - value;
		value = tmp;
	}
	return ++result;
}

template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
	if (first == last) return result;
	*result = *first;
	typedef typename JD::iterator_traits<InputIterator>::value_type v_t;
	v_t value = *first;
	while(++first != last) {
		v_t tmp = *first;
		*(++result) = binary_op(tmp, value);
		value = tmp;
	}
	return ++result;
}

template<class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, T init) {
	// 以第一个元素个数为据，将两个序列都走一遍
	for (; first1 != last1; ++first1, ++first2) {
		init = init + (*first1) * (*first2); // 一般内积
	}
	return init;
}

template<class InputIterator1, class InputIterator2, class T, class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, T init, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
	for (; first1 != last1; ++first1, ++first2) {
		init = binary_op1(init, binary_op2(*first1, *first2));
	}
	return init;
}

template<class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
	if (first == last) return result;
	*result = *first;
	typedef typename JD::iterator_traits<InputIterator>::value_type v_t;
	v_t value = *first;
	while (++first != last) {
		value = value + *first;
		*(++result) = value;
	}
	return ++result;
}

template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
	if (first == last) return result;
	*result = *first;
	typedef typename JD::iterator_traits<InputIterator>::value_type v_t;
	v_t value = *first;
	while (++first != last) {
		value = binary_op(value, *first);
		*(++result) = value;
	}
	return ++result;
}

template<class T>
inline T identity_element(std::multiplies<T>) {
	return T(1);
}

template<class T>
inline T identity_element(std::plus<T>) {
	return T(0);
}

// 幂次方
template<class T, class Interger, class MonoidOperation>
T power(T x, Interger n, MonoidOperation op) {
	if (n == 0) {
		return identity_element(op); // 取出 证同元素
	} else {
		while((n & 1) == 0) {
			n >>= 1;
			x = op(x, x);
		}

		T result = x;
		n >>= 1;
		while (n != 0) {
			x = op(x, x);
			if ((n & 1) != 0) {
				result = op(result, x);
			}
			n >>= 1;
		}
		return result;
	}
}

// 幂乘
template<class T, class Integer>
inline T power(T x, Integer n) {
	return power(x, n, std::multiplies<T>()); // 指定乘法
}

template<class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T value) {
	while(first != last) *first++ = value++;
}

JD_SPACE_END

#endif
