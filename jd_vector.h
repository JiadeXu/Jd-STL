/*************************************************************************
	> File Name: jd_vector.h
	> Author: 
	> Mail: 
	> Created Time: Sat Sep 11 19:45:55 2021
 ************************************************************************/

#ifndef _JD_VECTOR_H
#define _JD_VECTOR_H

#include "jd_macro.h"
#include "jd_alloc.h"
#include <cstddef>
#include <memory>

JD_SPACE_BEGIN

// class Alloc = std::allocator<T>
template<class T, class Alloc = JD::alloc>
class vector {
public:
	// vector的嵌套型别定义
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
public:
	typedef simple_alloc<value_type, Alloc> data_alloc;
};

JD_SPACE_END

#endif
