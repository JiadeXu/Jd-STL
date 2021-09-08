/*************************************************************************
	> File Name: 5.config.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed Sep  8 23:12:15 2021
 ************************************************************************/
// __STL_CLASS_PARTIAL_SPECIALIZATION
#include <iostream>
using namespace std;

// 一般化
template<class I, class O>
struct testClass {
	testClass() { cout << "I O" << endl; }
};

// 偏特化
template<class T>
struct testClass<T*, T*> {
	testClass() { cout << "T*, T*" << endl; }
};

// 偏特化
template<class T>
struct testClass<const T*, T*> {
	testClass() { cout << "const T*, T*" << endl; }
};

int main() {
	testClass<int, char> obj1;
	testClass<int*, int *> obj2;
	testClass<const int *, int *> obj3;
	return 0;
}