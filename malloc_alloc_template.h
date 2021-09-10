#ifndef _SIMPLE_ALLOC_
#define _SIMPLE_ALLOC_
#include <cstdlib>
#include "jd_macro.h"
// #include <malloc/_malloc.h>
#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined (__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC std::cerr << "out of memory" << std::endl; exit(1)
#endif

JD_SPACE_BEGIN

template<int inst>
class __malloc_alloc_template {
private:
    // 处理内存不足的情况
    // oom out of memory
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static void (* __malloc_alloc_oom_handler)(); // 类似于 set_new_handler设置的异常处理函数
public:
    static void *allocate(size_t n) {
        void *result = malloc(n);
        // 以下无法满足需求是 改用 oom_malloc
        if (result == 0) result = oom_malloc(n);
        return result;
    }
    static void deallocate(void *p, size_t) {
        free(p); // 第一级配置器直接用 free
    }
    static void *reallocate(void *p, size_t old_size, size_t new_sz) {
        void *result = realloc(p, new_sz);
        if (result == 0) oom_realloc(p, new_sz);
        return result;
    }
    // 模仿set_new_handler
    static void (* set_malloc_handler(void (*f)()))() {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }
};

// malloc_alloc out-of-memory handling
// 初值为0
template<int inst>
void (* __malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template<int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t n) {
    void (* my_malloc_handler)();
    void *result = 0;
    for (;;) {
        // 不断尝试释放、配置、再释放、再配置
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)(); // 调用处理例程，释放内存
        result = malloc(n); // 再次尝试分配内存空间
        if (result) return result;
    }
    return result;
}

template<int inst>
void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n) {
    
    void (* my_malloc_handler)();
    void *result = 0;
    for (;;) {
        // 不断尝试释放、配置、再释放、再配置
        my_malloc_handler = __malloc_alloc_oom_handler;
        if (my_malloc_handler == 0) { __THROW_BAD_ALLOC; }
        (*my_malloc_handler)(); // 调用处理例程，释放内存
        result = realloc(p, n); // 再次尝试分配内存空间
        if (result) return result;
    }
    return result;
}

typedef __malloc_alloc_template<0> malloc_alloc;

JD_SPACE_END

#endif