/*************************************************************************
	> File Name: test_rb_tree_r.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue Sep 28 15:18:05 2021
 ************************************************************************/

#include<iostream>
#include "../jd_rb_tree.h"
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
	JD::my_rb_tree<int, int, identity<int>, JD::JDLess<int> > itree, copy_t;
	// cout << itree.size() << endl;
	// int arr[] = {10, 7, 8, 15, 5, 6, 11, 13, 12};
	// // , 7, 8, 21, 3, 11
	// // std::__identity<int> k;
	// std::vector<int> xarr(arr, arr + 9);
	// xarr.push_back(17);
	// xarr.push_back(18);
	// xarr.push_back(19);
	// xarr.push_back(28);
	// for(auto i : xarr) {
	// 	itree.insert_unique(i);
	// }
	// cout << "begin " << *(itree.begin()) << endl;
	// itree.erase(itree.begin()); // bug  不能删除头节点的
	// cout << "size of tree " << itree.size() << endl;
	// for (auto i : itree) {
	// 	cout << i << " ";
	// }
	// cout << endl;
	// itree.preorder();

	// return 0;

	// cout << "erase start" << endl;
	// int ktd = 17;
	// itree.insert_equal(ktd);
	// cout << "count " << itree.count(ktd) << endl;
	// cout << "after erase " << ktd << " size of tree " << itree.size() << endl;
	// for (auto i : itree) {
	// 	cout << i << " ";
	// }
	// cout << endl;
	// itree.preorder();

	// copy_t = itree;

	// itree.erase(ktd);
	// cout << "after erase " << ktd << " size of tree " << itree.size() << endl;
	// for (auto i : itree) {
	// 	cout << i << " ";
	// }
	// cout << endl;
	// itree.preorder();
	// cout << "copy_t " << copy_t.size() << endl;
	
	// copy_t.preorder();
	// return 0;
}