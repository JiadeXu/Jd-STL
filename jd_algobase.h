/*************************************************************************
	> File Name: jd_algobase.h
	> Author: 
	> Mail: 
	> Created Time: Sun Sep 12 10:21:44 2021
 ************************************************************************/

#ifndef _JD_ALGOBASE_H
#define _JD_ALGOBASE_H

#include "jd_macro.h"

JD_SPACE_BEGIN

template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T &x) {
	OutputIterator cur = first;
	while(n--) {
		*(cur++) = x;
	}
	return first;
}

JD_SPACE_END

#endif
