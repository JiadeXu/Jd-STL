/*************************************************************************
	> File Name: jd_function.h
	> Author: 
	> Mail: 
	> Created Time: Wed Sep 29 23:39:58 2021
 ************************************************************************/

#ifndef _JD_FUNCTION_H
#define _JD_FUNCTION_H

#include "jd_macro.h"

JD_SPACE_BEGIN

template<class T>
struct identity {
	const T& operator()(const T &x) {
		return x;
	}
};

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

template <class _Arg, class _Result>
struct unary_function {
  typedef _Arg argument_type;
  typedef _Result result_type;
};


template<class _Pair>
struct select1st : public unary_function<_Pair, typename _Pair::first_type> {
	const typename _Pair::first_type& operator()(const _Pair &x) const {
		return x.first;
	}
};

JD_SPACE_END

#endif
