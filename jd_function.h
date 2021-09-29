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

JD_SPACE_END

#endif
