/*************************************************************************
	> File Name: 6algobase.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 16:42:22 2021
 ************************************************************************/

#include <algorithm>
#include <iostream>
#include "../jd_algobase.h"
#include "../jd_vector.h"
#include <functional>
#include <string>
#include <vector>
using namespace std;

template<class T>
struct display {
	void operator()(const T &x) const {
		cout << x << " ";
	}
};

int main() {
	int ia[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	JD::vector<int> iv1(ia, ia + 5);
	JD::vector<int> iv2(ia, ia + 9);
	// 判断两个区间的第一个不匹配点，返回一个由两个迭代器组成的pair
	// 第一个执行第一区间的不匹配点，第二个执行第二区间的不匹配点
	// 应该先判断迭代器是否不等于容器的end() 然后才可以做输出操作
	auto ans = JD::mismatch(iv1.begin(), iv1.end(), iv2.begin());
	cout << *(ans.first) << endl;
	cout << *(ans.second) << endl;

	cout << "equal(iv1.begin(), iv1.end(), iv2.begin()) is " << JD::equal(iv1.begin(), iv1.end(), iv2.begin()) << endl; // 1 true
	// {0, 1, 2, 3, 4} 小于 { 3 4 5 6 7 }
	cout << "equal(iv1.begin(), iv1.end(), &ia[3]) is " << JD::equal(iv1.begin(), iv1.end(), &ia[3]) << endl; // 0 false

	JD::fill(iv1.begin(), iv1.end(), 9);
	cout << "iv1 [0]: ";
	JD::for_each(iv1.begin(), iv1.end(), display<int>()); // 7 7 7 9 9
	cout << endl;

	JD::fill_n(iv1.begin(), 3, 7);
	cout << "iv1 fill_n(iv1.begin(), 3, 7): ";
	JD::for_each(iv1.begin(), iv1.end(), display<int>()); // 7 7 7 9 9
	cout << endl;

	auto ite1 = iv1.begin();
	auto ite2 = ite1;
	JD::advance(ite2, 3); // 前进3格
	JD::iter_swap(ite1, ite2);
	cout << "iter_swap " << *ite1 << " " << *ite2 << endl; // 9 7
	cout << "iv1 [1]: ";
	JD::for_each(iv1.begin(), iv1.end(), display<int>()); // 9 7 7 7 9
	cout << endl;

	cout << "max(*ite1, *ite2) is " << JD::max(*ite1, *ite2) << endl;
	cout << "min(*ite1, *ite2) is " << JD::min(*ite1, *ite2) << endl;

	// iv1: 9 7 7 7 9 iv2: 0 1 2 3 4 5 6 7 8
	JD::swap(*iv1.begin(), *iv2.begin());
	cout << "iv1 [2]: ";
	JD::for_each(iv1.begin(), iv1.end(), display<int>());
	cout << endl;

	cout << "iv2 [1]: ";
	JD::for_each(iv2.begin(), iv2.end(), display<int>());
	cout << endl;

	string stra1[] = {"Jamie", "JJHou", "Jason"};
	string stra2[] = {"Jamie", "JJhou", "Jerry"};

	cout << "stra1 < stra2 = " << JD::lexicographical_compare(stra1, stra1 + 2, stra2, stra2 + 2) << endl;
	cout << "stra1 > stra2 = " << JD::lexicographical_compare(stra1, stra1 + 2, stra2, stra2 + 2, greater<string>()) << endl;

	return 0;
}