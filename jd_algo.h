/*************************************************************************
	> File Name: jd_algo.h
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 00:12:05 2021
 ************************************************************************/

#ifndef _JD_ALGO_H
#define _JD_ALGO_H

#include "jd_iterator.h"
#include "jd_macro.h"

JD_SPACE_BEGIN

template<class Iterator, class T>
Iterator find(Iterator first, Iterator last, const T &value) {
	while(first != last && *first != value) {
		++first;
	}
	return first;
}

JD_SPACE_END

#endif
