/*************************************************************************
	> File Name: 6-7-n.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon Oct  4 15:05:44 2021
 ************************************************************************/

#include <functional>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <ostream>
#include "../jd_vector.h"
#include "../jd_algorithm.h"
using namespace std;

struct even {
	bool operator()(int x) const {
		return x % 2 ? false : true;
	}
};

int main() {
	int ia[] = { 12, 17, 20, 22, 23, 30, 33, 40 };
	JD::vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));
	cout << "lower_bound 21" << *JD::lower_bound(iv.begin(), iv.end(), 21) << endl; // 22
	cout << "upper_bound 21" << *JD::upper_bound(iv.begin(), iv.end(), 21) << endl; // 22
	cout << "lower_bound 22" << *JD::lower_bound(iv.begin(), iv.end(), 22) << endl; // 22
	cout << "upper_bound 22" << *JD::upper_bound(iv.begin(), iv.end(), 23) << endl; // 23

	cout << "bin_ser 33 " << JD::binary_search(iv.begin(), iv.end(), 33) << endl;
	cout << "bin_ser 34 " << JD::binary_search(iv.begin(), iv.end(), 34) << endl;

	// 下一个排列组合
	cout << "下一个排列组合 ";
	JD::next_permutation(iv.begin(), iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl; // 12 17 20 22 23 30 40 33

	// 上一个排列组合
	cout << "上一个排列组合 ";
	JD::prev_permutation(iv.begin(), iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl; // 12 17 20 22 23 30 33 40

	cout << "随机重排 ";
	JD::random_shuffle(iv.begin(), iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	
	cout << "partial_sort ";
	JD::partial_sort(iv.begin(), iv.begin() + 4, iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	cout << "sort 递增 ";
	JD::sort(iv.begin(), iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl; // 12 17 20 22 23 30 33 40

	cout << "sort 递减 ";
	JD::sort(iv.begin(), iv.end(), std::greater<int>());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl; // 40 33 30 23 22 20 17 12
	
	iv.push_back(22);
	iv.push_back(30);
	iv.push_back(17);

	cout << "stable_sort ";
	std::stable_sort(iv.begin(), iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl; // 12 17 17 20 22 22 23 30 30 33 40

	cout << "equal_range 22 ";
	auto parirIte = JD::equal_range(iv.begin(), iv.end(), 22);
	cout << *(parirIte.first) << " ~ " << *(parirIte.second) << endl; // 22 23
	
	cout << "equal_range 25 ";
	parirIte = JD::equal_range(iv.begin(), iv.end(), 25);
	cout << *(parirIte.first) << " ~ " << *(parirIte.second) << endl; // 30 30
	
	cout << "random_shuffle again ";
	JD::random_shuffle(iv.begin(), iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	cout << "将小于 " << *(iv.begin() + 5) << " 移动到该元素左边" ;
	JD::nth_element(iv.begin(), iv.begin() + 5, iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	cout << "2 - 将小于 " << *(iv.begin() + 5) << " 移动到该元素左边" ;
	JD::nth_element(iv.begin(), iv.begin() + 5, iv.end());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	cout << "stable_partition ";
	std::stable_partition(iv.begin(), iv.end(), even());
	JD::copy(iv.begin(), iv.end(), ostream_iterator<int>(cout, " "));
	cout << endl;


	return 0;
}