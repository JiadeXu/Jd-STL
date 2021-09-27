/*************************************************************************
	> File Name: 05_rbtree_test.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon Sep 27 10:16:51 2021
 ************************************************************************/

#include<iostream>
#include <type_traits>
#include <vector>
#include "../jd_algorithm.h"
#include <map>
#include "../jd_rb_tree.h"
using namespace std;

template<class T>
struct identity {
	const T& operator()(const T &x) {
		return x;
	}
};

int main() {
	JD::rb_tree<int, int, identity<int>, JD::JDLess<int> > itree;
	cout << itree.size() << endl;
	int arr[] = {10, 7, 8, 15, 5, 6, 11, 13, 12};
	// , 7, 8, 21, 3, 11
	// std::__identity<int> k;
	std::vector<int> xarr(arr, arr + 9);
	for (int i = 0; i < xarr.size(); i++) {
		auto rs = itree.insert_unique(xarr[i]);
		// cout << "insert " << arr[i] << " " << rs.second << " " << *(rs.first) << endl;
	}
	// 5 6 7 8 10 11 12 13 15 
	// 5(0) 6(1) 7(0) 8(1) 10(1) 11(0) 12(0) 13(1) 15(0) 
	cout << itree.size() << endl;
	auto itr = itree.begin();
	// for (int i = 0; i < itree.size(); i++, itr++) {
	// 	cout << *itr << " ";
	// }
	for (auto i = itree.begin(); i != itree.end(); i++) {
		cout << *i << " ";
	}
	cout << endl;
	JD::__rb_tree_base_iterator rbtite;
	for (auto i = itree.begin(); i != itree.end(); i++) {
		rbtite = JD::__rb_tree_base_iterator(i);
		cout << *i << "(" << rbtite.node->color << ") ";
	}
	cout << endl;

	itr = itree.find(10);
	cout << *itr << endl;
	itr.decreament();
	cout << *itr << endl;
	itr.increament();
	itr.increament();
	cout << *itr << endl;

	itr = itree.find(100);
	if (itr == itree.end()) {
		cout << "not found" << endl;
	}

	return 0;
}