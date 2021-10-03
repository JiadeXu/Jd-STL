/*************************************************************************
	> File Name: 6set-algorithm.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 21:47:17 2021
 ************************************************************************/

#include <functional>
#include <iostream>
#include <algorithm>
#include <iterator>
#include "../jd_set.h"
#include "../jd_algobase.h"
using namespace std;

template<class T>
struct display {
	void operator()(const T &x) const {
		cout << x << " ";
	}
};

int main() {
	int ia1[6] = {1, 3, 5, 7, 9, 11};
	int ia2[7] = {1, 1, 2, 3, 5, 8, 13};
	// std::multiplies<>
	JD::multiset<int> s1(ia1, ia1 + 6);
	JD::multiset<int> s2(ia2, ia2 + 7);

	cout << "set 1: ";
	JD::for_each(s1.begin(), s1.end(), display<int>());
	cout << endl;

	cout << "set 2: ";
	JD::for_each(s2.begin(), s2.end(), display<int>());
	cout << endl;

	auto first1 = s1.begin();
	auto last1 = s1.end();
	auto first2 = s2.begin();
	auto last2 = s2.end();

	cout << "union of s1 and s2: ";
	JD::set_union(first1, last1, first2, last2, ostream_iterator<int>(cout, " "));
	cout << endl;

	first1 = s1.begin();
	first2 = s2.begin();
	cout << "intersection of s1 and s2: ";
	JD::set_intersection(first1, last1, first2, last2, ostream_iterator<int>(cout, " "));
	cout << endl;

	first1 = s1.begin();
	first2 = s2.begin();
	cout << "difference of s1 and s2 (s1 - s2): ";
	JD::set_difference(first1, last1, first2, last2, ostream_iterator<int>(cout, " "));
	cout << endl;

	first1 = s1.begin();
	first2 = s2.begin();
	cout << "symmetric difference of s1 and s2 (s1 - s2): ";
	JD::set_symmetric_difference(first1, last1, first2, last2, ostream_iterator<int>(cout, " "));
	cout << endl;

	first1 = s1.begin();
	first2 = s2.begin();
	cout << "symmetric of s1 and s2 (s2 - s1): ";
	JD::set_difference(first2, last2, first1, last1, ostream_iterator<int>(cout, " "));
	cout << endl;
	return 0;
}