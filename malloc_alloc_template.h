#ifndef _SIMPLE_ALLOC_
#define _SIMPLE_ALLOC_
#include <cstddef>
#include <cstdlib>
#include <malloc/_malloc.h>
#include "jd_macro.h"
// 《STL源码剖析》p56
#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined (__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)
#endif

JD_SPACE_BEGIN
// malloc-based allocator 通常比稍后介绍的 default alloc 速度慢
template<int inst>
class __malloc_alloc_template {
private:
    // 处理内存不足的情况
    // oom out of memory
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_handler)(); // 类似于 set_new_handler设置的异常处理函数
public:
    static void* allocate(size_t n) {
        // void *result = 0;
        void *result = malloc(n);
        // 以下无法满足需求是 改用 oom_malloc
        if (result == 0) result = oom_malloc(n);
        return result;
    }

    static void deallocate(void *p, size_t) {
        free(p); // 第一级配置器直接使用free
    }

    static void *reallocate(void *p, size_t, size_t new_sz) {
        void *result = realloc(p, new_sz);
        if (result == 0) result = oom_realloc(p, new_sz);
        return result;
    }

    // 以下仿真C++的set_new_handler 可以通过它指定自己的out-of-memory handler
    static void (* set_malloc_handler(void (*f)()))() {
        // 不能用std::set_new_handler 因为它用的不是 c++ new 和 delete
        void (* old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }
};

// malloc_alloc out-of-memory handling
// 初值为0 有待客户端设定
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t size) {
    void (* my_malloc_handler)();
    void *result = 0;
    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        my_malloc_handler(); // 先调用用户设定的内存不足处理函数 整理内存
        // 再次尝试分配
        result = malloc(size);
        if (result) return result;
    }
    return result;
}

template<int inst>
void* __malloc_alloc_template<inst>::oom_realloc(void *p, size_t size) {
    void (* my_malloc_handler)();
    void *result = 0;
    for (;;) {
        my_malloc_handler = __malloc_alloc_oom_handler;
        my_malloc_handler(); // 先调用用户设定的内存不足处理函数 整理内存
        // 再次尝试分配
        result = realloc(p, size);
        if (result) return result;
    }
    return result;
}

typedef __malloc_alloc_template<0> malloc_alloc;

JD_SPACE_END

#endif