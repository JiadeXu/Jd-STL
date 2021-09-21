/*************************************************************************
	> File Name: 04.stack_queue.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue Sep 21 23:00:51 2021
 ************************************************************************/

#include<iostream>
#include "../jd_stack.h"
#include "../jd_queue.h"
#include <algorithm>
using namespace std;

// using namespace std;
#define SBEGIN(x) namespace x {
#define SEND(x) }

int main() {
	JD::stack<int> iStack;
	for(int i = 0; i < 5; i++) {
		iStack.push(2 * i + 1);
	}
	std::cout << "size " << iStack.size() << std::endl;
	while(!iStack.empty()) {
		std::cout << iStack.top() << std::endl;
		iStack.pop();
	}
	std::cout << "size " << iStack.size() << std::endl;

	JD::queue<int> que;
	for (int i = 0; i < 5; i++) {
		que.push(2 * i + 1);
	}

	std::cout << "size " << que.size() << std::endl;
	while(!que.empty()) {
		std::cout << que.front() << std::endl;
		que.pop();
	}
	std::cout << "size " << que.size() << std::endl;
	return 0;
}