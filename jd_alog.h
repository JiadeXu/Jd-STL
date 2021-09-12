/*************************************************************************
	> File Name: jd_alog.h
	> Author: 
	> Mail: 
	> Created Time: Sat Sep 11 14:48:49 2021
 ************************************************************************/

#ifndef _JD_ALOG_H
#define _JD_ALOG_H

template<class InputIterator, class T>
InputIterator find(InputIterator start, InputIterator end, const T &value) {
	while(start != end && *start != value) {
		++start;
	}
	return start;
}

#endif
