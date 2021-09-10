/*************************************************************************
	> File Name: jd_alloc.h
	> Author: 
	> Mail: 
	> Created Time: Fri Sep 10 00:38:09 2021
 ************************************************************************/

#ifndef _JD_ALLOC_H
#define _JD_ALLOC_H

#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>
#include "jd_macro.h"
#include "default_alloc_template.h"

JD_SPACE_BEGIN
template<class T>
inline T* _allocate(ptrdiff_t size, T *) {
	std::set_new_handler(0);
	T *tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
	if (tmp == 0) {
		std::cerr << "out of memory" << std::endl;
		exit(1);
	}
	return tmp;
}

template<class T>
inline void _deallocate(T *buffer) {
	::operator delete(buffer);
}

template<class T1, class T2>
inline void _construct(T1 *p, const T2 &value) {
	new(p) T1(value);
}

template<class T>
inline void _destory(T *ptr) {
	ptr->~T();
}

template<class T>
class allocator {
public:
	typedef T 			value_type;
	typedef T* 			pointer;
	typedef const T* 	const_pointer;
	typedef T& 			reference;
	typedef const T& 	const_reference;
	typedef size_t 		size_type;
	typedef ptrdiff_t 	difference_type;

	// rebind allocator of type U
	template<class U>
	struct rebind {
		typedef allocator<U> other;
	};

	// hint used for locality ref
	pointer allocate(size_type n, const void *hint = 0) {
		return _allocate((difference_type) n, (pointer)0);
	}

	void deallocate(pointer p, size_type n) { _deallocate(p); }

	void constructor(pointer p, const T &value) {
		_construct(p, value);
	}

	void destory(pointer p) {
		_destory(p);
	}

	pointer address(reference r) { return (pointer) &r; }

	const_pointer const_address(const_reference x) {
		return (const_pointer) &x;
	}

	size_type max_size() const {
		return size_type(UINT_MAX / sizeof(T));
	}
};

template<class T, class Alloc = __default_alloc_template<false, 1> >
class simple_alloc {
public:
	typedef T 			value_type;
	typedef T* 			pointer;
	typedef const T* 	const_pointer;
	typedef T& 			reference;
	typedef const T& 	const_reference;
	typedef size_t 		size_type;
	typedef ptrdiff_t 	difference_type;

	static T *allocate(size_t n) {
		return n == 0 ? 0 : (T*) Alloc::allocate(n * sizeof(T));
	}
	static T *allocate(void) {
		return Alloc::allocate(sizeof(T));
	}
	static void deallocate(T *p, size_t n) {
		Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T *p) {
		Alloc::deallocate(p, sizeof(T));
	}
};

JD_SPACE_END

#endif
