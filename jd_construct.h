/*************************************************************************
	> File Name: jd_construct.h
	> Author: 
	> Mail: 
	> Created Time: Sat Sep 11 20:32:07 2021
 ************************************************************************/

#ifndef _JD_CONSTRUCT_H
#define _JD_CONSTRUCT_H

#include "jd_macro.h"
#include "jd_type_traits.h"
#include "jd_iterator.h"
#include "jd_algobase.h"
#include <new>

JD_SPACE_BEGIN

template<class T1, class T2>
inline void construct(T1 *p, const T2 &value) {
	new (p) T1(value); // placement new 调用T1::T1(value)
}

// 以下是 destroy 的第一版本 接受一个指针
template<class T>
inline void destory(T *pointer) {
	pointer->~T(); // 调用 dtor ~T()
}

// destroy 第二版本 接受两个迭代器 此函数设法找出元素的数值型别
// 没有trivial destructors
template<class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __false_type) {
	for (; first != last; ++first) {
		destory(&*first);
	}
}
// 有trivial destructors 就什么也不做 不用调用析构
template<class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, __true_type) {}

// 判断元素数值型别 value_type 是否有 trivial destructor
template<class ForwardIterator, class T>
inline void __destory(ForwardIterator first, ForwardIterator last, T*) {
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	__destory_aux(first, last, trivial_destructor());
}

// 进而利用__type_traits<>求取适当措施
template<class ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last) {
	__destory(first, last, value_type(first));
}
// destroy对 char* wchar_t* 的特化
inline void destory(char *, char *) {}
inline void destory(wchar_t *, wchar_t *) {}

JD_SPACE_END

#endif
