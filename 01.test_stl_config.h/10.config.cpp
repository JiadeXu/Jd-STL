/*************************************************************************
	> File Name: 10.config.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed Sep  8 23:22:17 2021
 ************************************************************************/
// template能否根据前一个参数设置后一个参数的默认类型
#include <iostream>
#include <cstddef> // for size_t
using namespace std;

class alloc {};

template<class T, class Alloc = alloc, size_t bufsiz = 0>
class deque {
public:
	deque() { cout << "deque" << endl; }
};

// 根据前一个参数值T,设定下一个参数 Sequence的默认值为 deque<T>
template<class T, class Sequence = deque<T>>
class stack {
public:
	stack() { cout << "stack" << endl; }
private:
	Sequence c;
};

int main() {
	stack<int> s;
	// deque
	// stack
	return 0;
}