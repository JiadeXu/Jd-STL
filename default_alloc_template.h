/*************************************************************************
	> File Name: default_alloc_template.h
	> Author: 
	> Mail: 
	> Created Time: Fri Sep 10 11:43:38 2021
 ************************************************************************/
// 二级空间配置器 部分
#ifndef _DEFAULT_ALLOC_TEMPLATE_H
#define _DEFAULT_ALLOC_TEMPLATE_H
#include <cstddef>
#include "malloc_alloc_template.h"

JD_SPACE_BEGIN

enum { __ALIGN = 8 }; // 小型区块上调边界
enum { __MAX_BYTES = 128 }; // 小型区块上限
enum { __NFREELISTS = __MAX_BYTES / __ALIGN }; // free-lists 个数

// 二级配置器
// 注意，无“template” 参数
// 第一个参数用于多线程，书中不讨论多线程
template<bool threads, int inst>
class __default_alloc_template {
private:
	// 将bytes上调至8的倍数
	static size_t ROUD_UP(size_t bytes) {
		return (((bytes) + __ALIGN - 1) & ~(__ALIGN - 1));
	}
private:
	// 因为申请到的内存已经交给了客户端 所以二级配置器中就不需要再指向他们 因此free_list_link指向下一块
	// 书《STL源码剖析》p61
	union obj {
		union obj *free_list_link;
		char client_data[1]; /* 客户端申请的内存的实际区块地址 */
	};
private:
	// 16个 freelist；指针数组
	static obj * volatile free_list[__NFREELISTS];
	// 以下函数根据区块大小，决定使用第n号 free-list n 从0开始
	static size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes) + __ALIGN - 1) / __ALIGN - 1);
	}
	// 返回一个大小为n的对象 并可能加入大小为n的其他区块的free-list
	static void *refill(size_t n);
	// 配置一大块空间，可容纳nobjs个大小为 “size” 的区块
	// 如果配置nobjs个区块有所不便 nobjs 可能会降低
	static char *chunk_alloc(size_t size, int &nobjs);

	// chunk allocation state
	static char *start_free; // 内存池起始位置
	static char *end_free; // 结束位置
	static size_t heap_size;
public:
	static void *allocate(size_t n);
	static void deallocate(void *p, size_t n);
	static void *reallocate(void *p, size_t old_sz, size_t new_sz);
};

// 初始静态变量
template<bool threads, int inst>
char *__default_alloc_template<threads, inst>::start_free = 0;

template<bool threads, int inst>
char *__default_alloc_template<threads, inst>::end_free = 0;

template<bool threads, int inst>
size_t __default_alloc_template<threads, inst>::heap_size = 0;

// 8 16 24 32 40 48 56 64 72 80 88 96 104 112 120 128
template<bool threads, int inst>
typename __default_alloc_template<threads, inst>::obj * volatile __default_alloc_template<threads, inst>::free_list[__NFREELISTS] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

template<bool threads, int inst>
void *__default_alloc_template<threads, inst>::allocate(size_t n) {
	obj * volatile * my_free_list;
	obj *result;
	// 大于上限 128 就直接用一级配置器
	if (n > (size_t)__MAX_BYTES) {
		return malloc_alloc::allocate(n);
	}
	// 寻找16个 free list 中合适的
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	// 该位置未分配空间
	if (result == 0) {
		void *r = refill(ROUD_UP(n));
		return r;
	}
	// 调整free_list
	*my_free_list = result->free_list_link;
	return result;
}

template<bool threads, int inst>
void __default_alloc_template<threads, inst>::deallocate(void *p, size_t n) {
	obj* q = (obj*) p;
	obj * volatile * my_free_list;
	// 大于128byte就直接用一级配置器
	if (n > (size_t) __MAX_BYTES) {
		malloc_alloc::deallocate(p, n);
		return;
	}
	// 寻找对应的free list
	my_free_list = free_list + FREELIST_INDEX(n);
	// 回收内存
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}

template<bool threads, int inst>
void* __default_alloc_template<threads, inst>::refill(size_t n) {
	int nobjs = 20;
	// 调用chunk_alloc 尝试取得nobjs个区块作为free list的新节点
	char * chunk = chunk_alloc(n, nobjs);
	obj * volatile * my_free_list;
	obj *result;
	obj *current_obj, *next_obj;

	// 如果只获得一个区块，这个区块分配给调用者，free list无新节点
	if (nobjs == 1) return chunk;
	// 否则准备调整free list 纳入新节点
	my_free_list = free_list + FREELIST_INDEX(n);

	// 以下在 chunk 空间内建立 free list
	result = (obj *) chunk; // 这一块准备返回给客端
	// 以下导引free list指向新配置的空间（取自内存池）
	*my_free_list = next_obj = (obj *) (chunk + n);
	// 以下将free list的各节点串接起来
	for (int i = 1;; i++) {
		// 从1开始 因为第0个返回给了客端
		current_obj = next_obj;
		next_obj = (obj *)((char *)next_obj + n);
		if (i == nobjs - 1) {
			current_obj->free_list_link = 0;
			break;
		} else {
			current_obj->free_list_link = next_obj;
		}
	}
	return result;
}

// 从内存池中取内存
template<bool threads, int inst>
char* __default_alloc_template<threads, inst>::chunk_alloc(size_t size, int &nobjs) {
	char *result;
	size_t total_bytes = size * nobjs;
	size_t bytes_left = end_free - start_free; // 内存池剩余量

	if (bytes_left >= total_bytes) {
		// 内存池剩余空间完全满足需求量
		result = start_free;
		start_free += total_bytes;
		return result;
	} else if (bytes_left >= size) {
		// 内存剩余空间不能完全满足需求量 但是足够供应一个(含)以上的区块
		nobjs = bytes_left / size;
		total_bytes = nobjs * size;
		result = start_free;
		start_free += total_bytes;
		return result;
	} else {
		// 内存池剩余空间连一个区块的大小都无法供应
		size_t bytes_to_get = 2 * total_bytes + ROUD_UP(heap_size >> 4);
		// 以下试着让内存池的残余零头还有利用价值
		if (bytes_left > 0) {
			// 内存池还有一些零头、先配给适当的free list
			// 首先寻找适当的free list
			obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
			// 调整free list 将内存池中的残余空间编入
			((obj *) start_free)->free_list_link = *my_free_list;
			*my_free_list = (obj *) start_free;
		}
		//
		// 配置heap空间 用来补充内存池
		start_free = (char *)malloc(bytes_to_get);
		if (start_free == 0) {
			// heap 空间不足 malloc 失败
			obj * volatile * my_free_list, *p;
			// 试着检视我们手上拥有的东西，不能尝试配置
			// 较小的区块，因为那在多进程的机器上容易导致灾难
			// 以下搜寻适当的free list
			// 即 尚有未用区块，且区块够大
			for (int i = size; i <= __MAX_BYTES; i += __ALIGN) {
				my_free_list = free_list + FREELIST_INDEX(i);
				p = my_free_list;
				if (p != 0) {
					// free list 尚有未用的区块
					*my_free_list = p->free_list_link;
					// 将p空间返回给客端
					start_free = (char *) p;
					end_free = start_free + i;
					// 递归调用自己修正nobjs
					return chunk_alloc(size, nobjs);
					// 所有残余零头终将被编入适当的free-list中备用
				}
			}
			end_free = 0;
			// 实在没有内存可用了
			// 调用一级配置器 看看 out-of-memory机制能否尽点力
			start_free = (char *)malloc_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		// 递归调用自己 调整nobjs
		return chunk_alloc(size, nobjs);
	}
	// 不能有返回值 否则会出现段错误
	// return 0;
}

JD_SPACE_END
#endif
