/*************************************************************************
	> File Name: jd_algorithm.h
	> Author: 
	> Mail: 
	> Created Time: Thu Sep 23 23:14:15 2021
 ************************************************************************/

#ifndef _JD_ALGORITHM_H
#define _JD_ALGORITHM_H

#include "jd_iterator.h"
#include "jd_macro.h"
#include <cstdlib>
#include <iostream>
#include <utility>
#include "jd_function.h"
#include "jd_algobase.h"
#include "jd_tempbuf.h"

JD_SPACE_BEGIN
// ----------------------------------堆相关开始----------------------------------
template<class RandomAccessIterator, class Distance, class T, class Comp>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, const Comp &comp) {
	Distance parent = (holeIndex - 1) / 2; // 找出父节点
	while (holeIndex > topIndex && comp(*(first + parent), value)) {
		// 当尚未到达顶端 且 子节点值大于父节点
		*(first + holeIndex) = *(first + parent); // 对换父节点的值
		holeIndex = parent;
		parent = (holeIndex - 1) / 2; // 新洞的父节点
	}
	// 完成大顶堆 插入调整操作
	*(first + holeIndex) = value;
}

template<class RandomAccessIterator, class Distance, class T, class Comp>
inline void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance *, T *, const Comp &comp) {
	// 容器的最尾端，就是要调整元素的位置最大堆化 (last - first - 1)
	__push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)), comp);
}

template<class RandomAccessIterator, class Comp = JDLess<typename iterator_traits<RandomAccessIterator>::value_type> >
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Comp comp = Comp()) {
	// 注意 此函数调用时应该，新元素已位于容器底部
	__push_heap_aux(first, last, distance_type(first), value_type(first), comp);
}

// 源码剖析 书上原版
// template<class RandomAccessIterator, class Distance, class T, class Comp>
// void __adjust_heap_o(RandomAccessIterator first, Distance holeIndex, Distance len, T value, const Comp &comp) {
// 	Distance topIndex = holeIndex;
// 	Distance secondChild = 2 * holeIndex + 2; // 右边的子节点
// 	while(secondChild < len) {
// 		// 如果左节点比较大 就选左节点
// 		if (comp(*(first + secondChild), *(first + (secondChild - 1)))) {
// 			secondChild--;
// 		}
// 		// 令较大的值为洞值 并接着往下找
// 		*(first + holeIndex) = *(first + secondChild);
// 		holeIndex = secondChild;
// 		// 找出新洞的右子节点
// 		secondChild = 2 * (secondChild + 1);
// 	}
// 	// 是否有右子节点
// 	if (secondChild == len) { // 只有左子节点
// 		*(first + holeIndex) = *(first + (secondChild - 1));
// 		holeIndex = secondChild - 1;
// 	}
// 	__push_heap(first, holeIndex, topIndex, value, comp);
// }

template<class RandomAccessIterator, class Distance, class T, class Comp>
void __adjust_heap_jd(RandomAccessIterator first, Distance holeIndex, Distance len, T value, const Comp &comp) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * topIndex + 2; // 右边的子节点
	while(secondChild < len) {
		// 如果左节点比较大 就选左节点
		if (comp(*(first + secondChild), *(first + (secondChild - 1)))) {
			secondChild--;
		}
		if (comp(value, *(first + secondChild))) {
			*(first + topIndex) = *(first + secondChild);
			topIndex = secondChild;
			secondChild = 2 * topIndex + 2;
		} else {
			break;
		}
	}
	// 是否有右子节点
	if (secondChild == len) { // 只有左子节点
		if (comp(value, *(first + secondChild - 1))) {
			*(first + topIndex) = *(first + (secondChild - 1));
			topIndex = secondChild - 1;
		}
	}
	*(first + topIndex) = value;
}

template<class RandomAccessIterator, class T, class Distance, class Comp>
inline void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance *, const Comp &comp) {
	*result = *first; // 设定尾为首值 于是尾值即为欲求结果 可由客户稍后再调用 类似 pop_back的方法弹出末尾的值
	__adjust_heap_jd(first, Distance(0), Distance(last - first), value, comp);
}

template<class RandomAccessIterator, class T, class Comp>
inline void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, const Comp &comp) {
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first), comp);
}

template<class RandomAccessIterator, class Comp = JDLess<typename iterator_traits<RandomAccessIterator>::value_type> >
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, const Comp &comp = Comp()) {
	__pop_heap_aux(first, last, value_type(first), comp);
}

template<class RandomAccessIterator, class Comp = JDLess<typename iterator_traits<RandomAccessIterator>::value_type> >
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, const Comp &comp = Comp()) {
	// 每执行一次 pop_heap 次极值就被放到新尾端 
	while(last - first > 1) {
		pop_heap(first, last--, comp);
	}
}

template<class RandomAccessIterator, class T, class Distance, class Comp>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance *, const Comp &comp) {
	if (last - first < 2) return; // 长度小于2 不需要重新排序
	Distance len = last - first;
	// 找出第一个需要排序的子树头部，以parent标示出；建堆 从底向上
	Distance parent = (len - 2) / 2;
	while (true) {
		// 重新排列
		__adjust_heap_jd(first, parent, len, T(*(first + parent)), comp);
		if (parent == 0) {
			return;
		}
		parent--;
	}
}

template<class RandomAccessIterator, class Comp = JDLess<typename iterator_traits<RandomAccessIterator>::value_type> >
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, const Comp &comp = Comp()) {
	__make_heap(first, last, value_type(first), distance_type(first), comp);
}
// ----------------------------------堆相关结束----------------------------------

template<class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T &value, Distance *, JD::forward_iterator_tag) {
	Distance len = JD::distance(first, last);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		JD::advance(middle, half);
		if (*middle < value) {
			first = middle;
			++first;
			len = len - half - 1;
		} else {
			len = half;
		}
	}
	return first;
}

template<class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T &value, Distance *, JD::random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (*middle < value) {
			first = middle + 1;
			len = len - half - 1;
		} else {
			len = half;
		}
	}
	return first;
}

template<class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T &value) {
	return JD::__lower_bound(first, last, value, JD::distance_type(first), JD::iterator_category(first));
}

template<class ForwardIterator, class T, class Distance, class Compare>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp, Distance *, JD::forward_iterator_tag) {
	Distance len = JD::distance(first, last);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		JD::advance(middle, half);
		if (comp(*middle, value)) {
			first = middle;
			++first;
			len = len - half - 1;
		} else {
			len = half;
		}
	}
	return first;
}

template<class RandomAccessIterator, class T, class Distance, class Compare>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T &value, Compare comp, Distance *, JD::random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (comp(*middle, value)) {
			first = middle + 1;
			len = len - half - 1;
		} else {
			len = half;
		}
	}
	return first;
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp) {
	return JD::__lower_bound(first, last, value, comp, JD::distance_type(first), JD::iterator_category(first));
}

// upper
template<class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T &value, Distance *, JD::forward_iterator_tag) {
	Distance len = JD::distance(first, last);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		JD::advance(middle, half);
		if (value < *middle) {
			len = half;
		} else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}

template<class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T &value, Distance *, JD::random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (value < *middle) {
			len = half;
		} else {
			first = middle + 1;
			len = len - half - 1;
		}
	}
	return first;
}

template<class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T &value) {
	return JD::__upper_bound(first, last, value, JD::distance_type(first), JD::iterator_category(first));
}

template<class ForwardIterator, class T, class Distance, class Compare>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp, Distance *, JD::forward_iterator_tag) {
	Distance len = JD::distance(first, last);
	Distance half;
	ForwardIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first;
		JD::advance(middle, half);
		if (comp(value, *middle)) {
			len = half;
		} else {
			first = middle;
			++first;
			len = len - half - 1;
		}
	}
	return first;
}

template<class RandomAccessIterator, class T, class Distance, class Compare>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T &value, Compare comp, Distance *, JD::random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle;
	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (comp(value, *middle)) {
			len = half;
		} else {
			first = middle + 1;
			len = len - half - 1;
		}
	}
	return first;
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T &value, Compare comp) {
	return JD::__upper_bound(first, last, value, comp, JD::distance_type(first), JD::iterator_category(first));
}

template<class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T &value) {
	ForwardIterator i = lower_bound(first, last, value);
	return i != last && !(value < *i);
}

template<class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T &value, Compare comp) {
	ForwardIterator i = lower_bound(first, last, value, comp);
	return i != last && !(value < *i);
}

template<class BidirectionalIterator>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last) return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last) return false;
	i = last;
	--i;

	for (;;) {
		BidirectionalIterator ii = i;
		--i;
		if (*i < *ii) {
			BidirectionalIterator j = last;
			while(!(*i < *(--j))) {}
			JD::iter_swap(i, j);
			JD::reverse(ii, last);
			return true;
		}
		if (i == last) {
			JD::reverse(first, last);
			return false;
		}
	}
}

template<class BidirectionalIterator, class Compare>
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
	if (first == last) return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last) return false;
	i = last;
	--i;

	for (;;) {
		BidirectionalIterator ii = i;
		--i;
		if (comp(*i, *ii)) {
			BidirectionalIterator j = last;
			while(!(comp(*i, *(--j)))) {}
			JD::iter_swap(i, j);
			JD::reverse(ii, last);
			return true;
		}
		if (i == last) {
			JD::reverse(first, last);
			return false;
		}
	}
}

template<class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last) {
	if (first == last) return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last) return false;
	i = last;
	--i;

	for (;;) {
		BidirectionalIterator ii = i;
		--i;
		if (*ii < *i) {
			BidirectionalIterator j = last;
			while(!(*(--j) < *i)) {}
			JD::iter_swap(i, j);
			JD::reverse(ii, last);
			return true;
		}
		if (i == last) {
			JD::reverse(first, last);
			return false;
		}
	}
}

template<class BidirectionalIterator, class Compare>
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp) {
	if (first == last) return false;
	BidirectionalIterator i = first;
	++i;
	if (i == last) return false;
	i = last;
	--i;

	for (;;) {
		BidirectionalIterator ii = i;
		--i;
		if (comp(*ii, *i)) {
			BidirectionalIterator j = last;
			while(!(comp(*(--j), *i))) {}
			JD::iter_swap(i, j);
			JD::reverse(ii, last);
			return true;
		}
		if (i == last) {
			JD::reverse(first, last);
			return false;
		}
	}
}

template<class RandomAccessIterator, class Distance>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance *) {
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i) {
		// JD::iter_swap(i, first + Distance(lrand48() % ((i - first) + 1)));
		JD::iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
	}
}

template<class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last) {
	JD::__random_shuffle(first, last, JD::distance_type(first));
}

template<class RandomAccessIterator, class RandomNumberGenerator>
inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator gen_rand) {
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i) {
		JD::iter_swap(i, first + gen_rand((i - first) + 1));
	}
}

template<class RandomAccessIterator, class T, class Compare = JDLess<typename iterator_traits<RandomAccessIterator>::value_type>>
void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T *, Compare comp = Compare()) {
	JD::make_heap(first, middle);
	for (RandomAccessIterator i = middle; i < last; ++i) {
		if (*i < *first) {
			JD::__pop_heap(first, middle, i, T(*i), JD::distance_type(first), comp);
			// JD::pop_heap(first, middle);
		}
	}
	JD::sort_heap(first, middle);
}

template<class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last) {
	JD::__partial_sort(first, middle, last, JD::value_type(first));
}

template<class RandomAccessIterator, class Compare>
inline void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, Compare comp) {
	JD::__partial_sort(first, middle, last, JD::value_type(first), comp);
}

// 排序
template<class RandomAccessIterator, class T, class Compare>
void __unguarded_linear_insert(RandomAccessIterator last, T value, Compare comp) {
	RandomAccessIterator next = last;
	--next;
	// 一旦没有出现逆转对 循环即可结束
	while (comp(value, *next)) {
		*last = *next;
		last = next;
		--next;
	}
	*last = value;
}

template<class RandomAccessIterator, class T, class Compare>
inline void __linear_insert(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp) {
	T value = *last;
	if (comp(value, *first)) {
		JD::copy_backward(first, last, last + 1);
		*first = value;
	} else {
		// 尾不小于头
		JD::__unguarded_linear_insert(last, value, comp);
	}
}

template<class RandomAccessIterator, class Compare>
void __insert_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	if (first == last) return;
	for (RandomAccessIterator i = first + 1; i != last; ++i) {
		JD::__linear_insert(first, i, JD::value_type(first), comp);
	}
}

template<class T, class Compare>
inline const T& __median(const T &a, const T &b, const T &c, Compare comp) {
	if (comp(a, b)) {
		if (comp(b, c)) // a < b < c
			return b;
		else if (comp(a, c)) // a < b b >= c a < c
			return c;
		else
		 	return a;
	} else if (comp(a, c)) {
		return a;
	} else if (comp(b, c)) {
		return c;
	} else {
		return b;
	}
}

template<class RandomAccessIterator, class T, class Compare>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, T pivot, Compare comp) {
	for (;;) {
		while(comp(*first, pivot)) ++first; // 找到 >= pivot的元素
		--last;
		while (comp(pivot, *last)) --last; // 找到 <= pivot的
		if (!(first < last)) return first; // 交错
		JD::iter_swap(first, last);
		++first; // 调整
	}
}

// 找出2 ^ k <= n的最大值 n = 7 k = 2; n = 20 k = 4; n = 8 k = 3
template<class Size>
inline Size __lg(Size n) {
	Size k;
	for (k = 0; n > 1; n >>= 1) ++k;
	return k;
}

// 当元素个数为40是 __introsort_loop 的最后一个参数将是 5 * 2 表示最多允许分割10层
template<class RandomAccessIterator, class T, class Size, class Compare>
void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size depth_limit, Compare comp) {
	while (last - first > __jd_threshold) {
		if (depth_limit == 0) {
			JD::partial_sort(first, last, last, comp); // 改用heapsort
			return;
		}
		--depth_limit;
		RandomAccessIterator cut = JD::__unguarded_partition(
			first, 
			last, 
			T(
				JD::__median(
					*first, 
					*(first + (last - first) / 2), 
					*(last - 1),
					comp
				)
			),
			comp
		);

		// 对右半段递归进行 sort
		JD::__introsort_loop(cut, last, JD::value_type(first), depth_limit, comp);
		// 第552再对左半段进行处理
		last = cut;
	}
}

template<class RandomAccessIterator, class Compare>
inline void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	typedef typename JD::iterator_traits<RandomAccessIterator>::value_type v_t;
	for (RandomAccessIterator i = first; i != last; ++i) {
		JD::__unguarded_linear_insert(i, v_t(*i), comp);
	}
}

template<class RandomAccessIterator, class Compare>
void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	if (last - first > __jd_threshold) {
		JD::__insert_sort(first, first + __jd_threshold, comp);
		JD::__unguarded_insertion_sort(first + __jd_threshold, last, comp);
	} else {
		JD::__insert_sort(first, last, comp);
	}
}

template<class RandomAccessIterator, class Compare>
inline void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
	if (first != last) {
		JD::__introsort_loop(first, last, JD::value_type(first), JD::__lg(last - first) * 2, comp);
		JD::__final_insertion_sort(first, last, comp);
	}
}

template<class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
	JD::sort(first, last, JD::JDLess<typename JD::iterator_traits<RandomAccessIterator>::value_type>());
}

template<class RandomAccessIterator, class T, class Distance>
std::pair<RandomAccessIterator, RandomAccessIterator> __equal_range(RandomAccessIterator first, RandomAccessIterator last, const T &value, Distance *, JD::random_access_iterator_tag) {
	Distance len = last - first;
	Distance half;
	RandomAccessIterator middle, left, right;

	while (len > 0) {
		half = len >> 1;
		middle = first + half;
		if (*middle < value) {
			first = middle + 1;
			len = len - half - 1;
		} else if (value < *middle) {
			len = half;
		} else {
			// 在前半段找 lower
			left = JD::lower_bound(first, middle, value);
			// 后半段找 upper
			right = JD::upper_bound(++middle, first + len, value);
			return std::pair<RandomAccessIterator, RandomAccessIterator>(left, right);
		}
	}
	// 区间内没有匹配的值
	return std::pair<RandomAccessIterator, RandomAccessIterator>(first, first);
}

template<class ForwardIterator, class T, class Distance>
std::pair<ForwardIterator, ForwardIterator> __equal_range(ForwardIterator first, ForwardIterator last, const T &value, Distance *, JD::forward_iterator_tag) {
	Distance len = JD::distance(first, last);
	Distance half;
	ForwardIterator middle, left, right;

	while (len > 0) {
		half = len >> 1;
		middle = first;
		JD::advance(middle, half);
		if (*middle < value) {
			first = middle;
			++first;
			len = len - half - 1;
		} else if (value < *middle) {
			len = half;
		} else {
			// 在前半段找 lower
			left = JD::lower_bound(first, middle, value);
			JD::advance(first, len);
			// 后半段找 upper
			right = JD::upper_bound(++middle, first, value);
			return std::pair<ForwardIterator, ForwardIterator>(left, right);
		}
	}
	// 区间内没有匹配的值
	return std::pair<ForwardIterator, ForwardIterator>(first, first);
}

template<class ForwardIterator, class T>
inline std::pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T &value) {
	return JD::__equal_range(first, last, value, JD::distance_type(first), JD::iterator_category(first));
}

template<class BidirectionalIterator1, class BidirectionalIterator2, class Distance>
BidirectionalIterator1 __rotate_adaptive(BidirectionalIterator1 first, BidirectionalIterator1 middle, BidirectionalIterator1 last, Distance len1, Distance len2, BidirectionalIterator2 buffer, Distance buffer_size) {
	BidirectionalIterator2 buffer_end;
	if (len1 > len2 && len2 <= buffer_size) {
		// 缓冲区足够安置序列二 （较短）
		buffer_end = JD::copy(middle, last, buffer);
		JD::copy_backward(first, middle, last);
		return JD::copy(buffer, buffer_end, first);
	} else if (len1 <= buffer_size) {
		// 足够缓冲 序列1
		buffer_end = JD::copy(first, middle, buffer);
		JD::copy(middle, last, first);
		return JD::copy_backward(buffer, buffer_end, last);
	} else {
		// 缓冲区不足
		JD::rotate(first, middle, last);
		JD::advance(first, len2);
		return first;
	}
}

template<class BidirectionalIterator, class Distance, class Pointer>
void __merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Distance len1, Distance len2, Pointer buffer, Distance buffer_size) {
	if (len1 <= len2 && len1 <= buffer_size) {
		// 缓冲区足够安置序列1
		Pointer end_buffer = JD::copy(first, middle, buffer);
		JD::merge(buffer, end_buffer, middle, last, first);
	} else if (len2 <= buffer_size) {
		// 缓冲区足够安置序列2
		Pointer end_buffer = JD::copy(first, middle, buffer);
		JD::__merge_backward(first, middle, buffer, end_buffer, last);
	} else {
		// 缓冲区不够安置任意一个序列
		BidirectionalIterator first_cur = first;
		BidirectionalIterator second_cur = middle;
		Distance len11 = 0;
		Distance len22 = 0;
		if (len1 > len2) {
			len11 = len1 / 2;
			JD::advance(first_cur, len11);
			second_cur = JD::lower_bound(middle, last, *first_cur);
			len22 = JD::distance(middle, second_cur);
		} else {
			len22 = len2 / 2;
			JD::advance(second_cur, len22);
			first_cur = JD::upper_bound(first, middle, *second_cur);
			len11 = JD::distance(first, first_cur);
		}
		BidirectionalIterator new_middle = JD::__rotate_adaptive(first_cur, middle, second_cur, len1 - len11, len2, buffer, buffer_size);

		// 针对左端
		JD::__merge_adaptive(first, first_cur, new_middle, len11, len22, buffer, buffer_size);
		// 针对右端
		JD::__merge_adaptive(new_middle, second_cur, last, len1 - len11, len2 - len22, buffer, buffer_size);
	}
}

template <class BidirectionalIterator, class Distance>
void __merge_without_buffer(BidirectionalIterator first,
                            BidirectionalIterator middle,
                            BidirectionalIterator last,
                            Distance len1, Distance len2) {
  if (len1 == 0 || len2 == 0)
    return;
  if (len1 + len2 == 2) {
    if (*middle < *first)
      JD::iter_swap(first, middle);
    return;
  }
  BidirectionalIterator first_cut = first;
  BidirectionalIterator second_cut = middle;
  Distance len11 = 0;
  Distance len22 = 0;
  if (len1 > len2) {
    len11 = len1 / 2;
    JD::advance(first_cut, len11);
    second_cut = JD::lower_bound(middle, last, *first_cut);
    len22 = JD::distance(middle, second_cut);
  }
  else {
    len22 = len2 / 2;
    JD::advance(second_cut, len22);
    first_cut = JD::upper_bound(first, middle, *second_cut);
    len11 = JD::distance(first, first_cut);
  }
  BidirectionalIterator __new_middle = JD::rotate(first_cut, middle, second_cut);
  __merge_without_buffer(first, first_cut, __new_middle, len11, len22);
  __merge_without_buffer(__new_middle, second_cut, last, len1 - len11, len2 - len22);
}

template<class BidirectionalIterator, class T, class Distance>
inline void __inplace_merge_aux(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, T*, Distance *) {
	Distance len1 = JD::distance(first, middle);
	Distance len2 = JD::distance(middle, last);

	JD::temporary_buffer<BidirectionalIterator, T> buf(first, last);
	if (buf.begin() == 0) {
		JD::__merge_without_buffer(first, middle, last, len1, len2);
	} else {
		JD::__merge_adaptive(first, middle, last, len1, len2, buf.begin(), Distance(buf.size()));
	}
}

template<class BidirectionalIterator>
inline void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last) {
	if (first == middle || middle == last) return;
	JD::__inplace_merge_aux(first, middle, last, JD::value_type(first), JD::distance_type(first));
}

template<class RandomAccessIterator, class Compare = JD::JDLess<typename JD::iterator_traits<RandomAccessIterator>::value_type> >
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare comp = Compare()) {
	typedef typename JD::iterator_traits<RandomAccessIterator>::value_type v_t;
	while (last - first > 3) {
		RandomAccessIterator cut = JD::__unguarded_partition(
			first, last, v_t(
				JD::__median(*first,
					*(first + (last - first) / 2),
					*(last - 1), comp)
			),
			comp);
		if (cut <= nth) {
			first = cut;
		} else {
			last = cut;
		}
	}
	JD::__insert_sort(first, last, comp);
}

template<class BidirectionalIterator>
void mergesort (BidirectionalIterator first, BidirectionalIterator last) {
	typename JD::iterator_traits<BidirectionalIterator>::difference_type n = JD::distance(first, last);
	if (n == 0 || n == 1) {
		return;
	} else {
		BidirectionalIterator mid = first + n / 2;
		JD::mergesort(first, mid);
		JD::mergesort(mid, last);
		JD::inplace_merge(first, mid, last);
	}
}

JD_SPACE_END

#endif
