/*************************************************************************
	> File Name: jd_type_traits.h
	> Author: 
	> Mail: 
	> Created Time: Sat Sep 11 21:03:33 2021
 ************************************************************************/

#ifndef _JD_TYPE_TRAITS_H
#define _JD_TYPE_TRAITS_H

#include "jd_macro.h"

JD_SPACE_BEGIN

template<class T>
struct __type_traits {
	/* 不要移除这个成员 它通知 ”有能力自动将 __type_traits 特化“的编译器说，
	我们现在所看到的这个 __type_traits template 是特殊的，这是为了确保万一
	编译器也使用一个名为 __type_traits 而其实与此处定义并无任何关联的template
	时，所有事情都将仍将顺利运作
	 */
	typedef __true_type this_dummy_member_must_be_first;
	/*
	可以移出以下任一个 但不能重新命名而却没有改变编译器中的对应名称
	 */
	typedef __false_type has_tirvial_default_constructor;
	typedef __false_type has_tirvial_copy_constructor;
	typedef __false_type has_tirvial_assignment_operator;
	typedef __false_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<char> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<signed char> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<unsigned char> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<short> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<unsigned short> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<int> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<unsigned int> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<long> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<unsigned long> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<float> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<double> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

__JD_STL_TEMPLATE_NULL struct __type_traits<long double> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

template<class T>
struct __type_traits<T*> {
	typedef __true_type has_tirvial_default_constructor;
	typedef __true_type has_tirvial_copy_constructor;
	typedef __true_type has_tirvial_assignment_operator;
	typedef __true_type is_POD_type;
};

JD_SPACE_END

#endif
