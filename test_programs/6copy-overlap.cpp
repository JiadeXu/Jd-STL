/*************************************************************************
	> File Name: 6copy-overlap.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 19:18:13 2021
 ************************************************************************/

#include <algorithm>
#include<iostream>
#include <deque>
#include "../jd_deque.h"
#include "../jd_algobase.h"
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

		JD::copy(ia + 2, ia + 7, ia);
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
		auto rs = id.begin();
		cout << "rs : " << *rs << endl;
		JD::copy(a, b, rs);
		cout << "deque copy 输出区间终点与输入区间重叠 "; // 2 3 4 5 6 5 6 7 8
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
		JD::advance(rs, 4);
		cout << "rs advance 4 : " << *rs << endl;
		JD::copy(a, b, rs);
		cout << "deque copy 输出区间的起点与输入区间重叠，可能会有问题 ";
		JD::for_each(id.begin(), id.end(), display<int>());
		cout << endl;
	}
	return 0;
}