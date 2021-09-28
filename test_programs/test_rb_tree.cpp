/*************************************************************************
	> File Name: test_rb_tree_r.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue Sep 28 15:18:05 2021
 ************************************************************************/

#include<iostream>
#include "../rb_tree.h"
#include <vector>
#include "../jd_algorithm.h"
using namespace std;

template<class T>
struct identity {
	const T& operator()(const T &x) {
		return x;
	}
};

int main() {
	JD::jd_rb_tree<int, int, identity<int>, JD::JDLess<int> > itree;
	cout << itree.size() << endl;
	int arr[] = {10, 7, 8, 15, 5, 6, 11, 13, 12};
	// , 7, 8, 21, 3, 11
	// std::__identity<int> k;
	std::vector<int> xarr(arr, arr + 9);
	xarr.push_back(17);
	xarr.push_back(18);
	xarr.push_back(19);
	xarr.push_back(28);
	for(auto i : xarr) {
		itree.insert_unique(i);
	}
	cout << "size of tree " << itree.size() << endl;
	for (auto i : itree) {
		cout << i << " ";
	}
	cout << endl;
	itree.preorder();

	cout << "erase start" << endl;
	itree.erase(19);
	cout << "after erase size of tree " << itree.size() << endl;
	for (auto i : itree) {
		cout << i << " ";
	}
	cout << endl;
	itree.preorder();
	return 0;
}