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
#include <iostream>

JD_SPACE_BEGIN
template<class V>
struct JDLess {
	bool operator()(const V &x, const V &y) const {
		return x < y;
	}
};
template<class V>
struct JDGreat {
	bool operator()(const V &x, const V &y) const {
		return x > y;
	}
};

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

JD_SPACE_END

#endif
