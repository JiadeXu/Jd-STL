/*************************************************************************
	> File Name: jd_hash_fun.h
	> Author: 
	> Mail: 
	> Created Time: Sat Oct  2 14:02:37 2021
 ************************************************************************/

#ifndef _JD_HASH_FUN_H
#define _JD_HASH_FUN_H

#include "jd_macro.h"
#include <cstddef>

JD_SPACE_BEGIN

template<class key>
struct my_hash {};

inline size_t _jd_hash_string(const char *s) {
	unsigned long h = 0;
	for (; *s; ++s) {
		h = 5 * h + *s;
	}
	return size_t(h);
}

template<>
struct my_hash<char *> {
	size_t operator()(const char *s) const {
		return _jd_hash_string(s);
	}
};

template<>
struct my_hash<const char *> {
	size_t operator()(const char *s) const {
		return _jd_hash_string(s);
	}
};

template<>
struct my_hash<char> {
	size_t operator()(char x) const {
		return x;
	}
};

template<>
struct my_hash<unsigned char> {
	size_t operator()(unsigned char x) const {
		return x;
	}
};

template<>
struct my_hash<signed char> {
	size_t operator()(signed char x) const {
		return x;
	}
};

template<>
struct my_hash<short> {
	size_t operator()(short x) const {
		return x;
	}
};

template<>
struct my_hash<unsigned short> {
	size_t operator()(unsigned short x) const {
		return x;
	}
};

template<>
struct my_hash<int> {
	size_t operator()(int x) const {
		return x;
	}
};

template<>
struct my_hash<unsigned int> {
	size_t operator()(unsigned int x) const {
		return x;
	}
};

template<>
struct my_hash<long> {
	size_t operator()(long x) const {
		return x;
	}
};

template<>
struct my_hash<unsigned long> {
	size_t operator()(unsigned long x) const {
		return x;
	}
};

JD_SPACE_END

#endif
