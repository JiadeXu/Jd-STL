/*************************************************************************
	> File Name: 6copy_backward_overlap.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 20:44:08 2021
 ************************************************************************/

#include <iostream>
#include "../jd_deque.h"
#include "../jd_algobase.h"
#include <algorithm>
using namespace std;

template<class T>
struct display {
	void operator()(const T &x) const {
		cout << x << " ";
	}
};

int main() {
	{
		cout << "Block 0" << endl;
		int ia[] = {0, 1, 2, 3, 4, 5 ,6, 7, 8};
		JD::copy_backward(ia + 2, ia + 7, ia + 5);
		cout << "ia copy "; // 2 3 4 5 6 5 6 7 8
		JD::for_each(ia, ia + 9, display<int>());
		cout << endl;
	}

	{
		cout << "Block 1" << endl;
		int ia[] = {0, 1, 2, 3, 4, 5 ,6, 7, 8};
		JD::deque<int> id(ia, ia + 9);
		auto a = id.begin();
		++(++a);
		auto b = id.end();
		----b;
		cout << "++++a " << *a << " " << "----b " << *b << endl;
		auto rs = id.end();
		cout << "rs : " << *rs << endl;
		JD::copy_backward(a, b, rs);
		cout << "deque copy 输出区间终点与输入区间重叠 "; // 0 1 2 3 2 3 4 5 6
		JD::for_each(id.begin(), id.end(), display<int>());
		cout << endl;
	}

	{
		cout << "Block 2" << endl;
		int ia[] = {0, 1, 2, 3, 4, 5 ,6, 7, 8};
		JD::deque<int> id(ia, ia + 9);
		auto a = id.begin();
		++(++a);
		auto b = id.end();
		----b;
		cout << "++++a " << *a << " " << "----b " << *b << endl;
		auto rs = id.begin();
		JD::advance(rs, 5);
		cout << "rs advance 5 : " << *rs << endl; // 5
		JD::copy_backward(a, b, rs);
		cout << "deque copy 输出区间的起点与输入区间重叠，可能会有问题 ";
		JD::for_each(id.begin(), id.end(), display<int>()); // 6 5 6 5 6 5 6 7 8
		cout << endl;
	}
	return 0;
}