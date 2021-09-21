/*************************************************************************
	> File Name: 03.deque.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue Sep 21 02:55:18 2021
 ************************************************************************/

#include<iostream>
#include <deque>
#include <algorithm>
#include "../jd_alloc.h"
#include "../jd_deque.h"

// using namespace std;
#define SBEGIN(x) namespace x {
#define SEND(x) }

SBEGIN(t1)

// using namespace std;
int main() {
	std::deque<int> ideq(20, 9);
	std::cout << "size = " << ideq.size() << std::endl;
	// 20 个 int 元素初始值为9
	// 缓冲区大小为32字节

	// 为每一个元素设定新值
	for (int i = 0; i < ideq.size(); i++) {
		ideq[i] = i;
	}

	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;

	// 追加3个元素
	for (int i = 0; i < 3; i++) {
		ideq.push_back(i);
	}
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	std::cout <<  "size = " << ideq.size() << std::endl;

	ideq.push_back(3);
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	ideq.push_front(99);
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	
	ideq.push_front(99);
	ideq.push_front(97);
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	// 搜寻99
	// std::deque<int>::iterator itr;
	// itr = std::find(ideq.begin(), ideq.end(), 99);
	// std::cout << *itr << std::endl;
	
	return 0;
}
SEND(t1)

SBEGIN(t2)
int main() {
	JD::deque<int, JD::alloc, 8> ideq(20, 9);
	std::cout << "size = " << ideq.size() << std::endl;
	// 20 个 int 元素初始值为9
	// 缓冲区大小为32字节

	// 为每一个元素设定新值
	for (int i = 0; i < ideq.size(); i++) {
		ideq[i] = i;
	}

	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;

	// 追加3个元素
	for (int i = 0; i < 3; i++) {
		ideq.push_back(i);
	}
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	std::cout <<  "size = " << ideq.size() << std::endl;

	ideq.push_back(3);
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	ideq.push_front(99);
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	
	ideq.push_front(99);
	ideq.push_front(97);
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "pop front back ";
	for (int i = 0; i < 3; i++) {
		ideq.pop_front();
	}
	for (int i = 0; i < 3; i++) {
		ideq.pop_back();
	}
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "insert " << *ideq.insert(ideq.begin() + 3, 93) << std::endl;
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "erase " << *ideq.erase(ideq.begin() + 4, ideq.begin() + 10) << std::endl;
	for (int i = 0; i < ideq.size(); i++) {
		std::cout << ideq[i] << " ";
	}
	std::cout << std::endl;
	ideq.clear();
	std::cout << ideq.size() << std::endl;
	return 0;
}
SEND(t2)

int main() {
	t1::main();
	std::cout << "t2" << std::endl;
	t2::main();
	return 0;
}