/*************************************************************************
	> File Name: lconfig-null-template-arguments.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed Sep  8 23:30:23 2021
 ************************************************************************/
// stl_config.h 中宏__STL_NULL_TEMPLATE_ARGS的定义为
// #ifdef __STL_EXPLICIT_FUNCTION_TMPL_ARGS
// #	define __STL_NULL_TMPL_AGRS <>
// #else
// #	define __STL_NULL_TMPL_AGRS
// #endif

#include <iostream>
#include <cstddef>
using namespace std;

class alloc {};

template<class T, class Alloc = alloc, size_t bufsiz = 0>
class deque {
public:
	deque() { cout << "deque" << endl; }
};

template<class T, class Sequence>
class stack;

template<class T, class Sequence>
bool operator==(const stack<T, Sequence> &x, const stack<T, Sequence> &y);

template<class T, class Sequence>
bool operator<(const stack<T, Sequence> &x, const stack<T, Sequence> &y);

template<class T, class Sequence = deque<T> >
class stack {
	// 正常是这样的
	// friend bool operator== <T>(const stack<T> &, const stack<T> &);
	// friend bool operator< <T>(const stack<T> &, const stack<T> &);
	// 但可以写成这样
	friend bool operator== <>(const stack<T> &, const stack<T> &);
	friend bool operator< <>(const stack<T> &, const stack<T> &);
	// 书里写的是这样不行
	// friend bool operator== (const stack<T> &, const stack<T> &);
	// friend bool operator< (const stack<T> &, const stack<T> &);
public:
	stack() { cout << "stack" << endl; }
private:
	Sequence c;
};


template<class T, class Sequence>
bool operator==(const stack<T, Sequence> &x, const stack<T, Sequence> &y) {
	cout << "operator== " << '\t';
	return true;
}

template<class T, class Sequence>
bool operator<(const stack<T, Sequence> &x, const stack<T, Sequence> &y) {
	cout << "operator< " << '\t';
	return true;
}

int main() {
	stack<int> x;
	stack<int> y;

	cout << (x < y) << endl;
	cout << (x == y) << endl;
	return 0;
}