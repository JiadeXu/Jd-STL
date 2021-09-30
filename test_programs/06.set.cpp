/*************************************************************************
	> File Name: 06.set.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed Sep 29 23:57:36 2021
 ************************************************************************/

#include<iostream>
#include "../jd_set.h"
#include <vector>
using namespace std;

void display(JD::set<int> &jset) {
	for (auto i : jset) {
		cout << i << " ";
	}
	cout << endl;

	// auto tr = jset.begin();
	// for (int i = 0; i < jset.size(); i++) {
	// 	cout << *(tr++) << ' ';
	// }
	// cout << endl;
}

int main() {
	int arr[] = {10, 7, 8, 15, 5, 6, 11, 13, 12};
	vector<int> vec(arr, arr + 9);
	JD::set<int> jset(vec.begin(), vec.end());
	cout << jset.size() << endl;
	jset.insert(89);
	cout << "show 1" << endl;
	display(jset);
	jset.show();
	jset.erase(jset.begin()); //删除了开头的5
	cout << "erase " << jset.erase(11) << endl;
	cout << "show 2" << endl;
	display(jset);
	jset.show();
	cout << "erase [0, 3)" << endl;
	auto itr = jset.begin();
	itr.increament();
	itr.increament();
	itr.increament();
	jset.erase(jset.begin(), itr);
	cout << "show 3" << endl;
	jset.show();
	display(jset);
	jset.clear();
	cout << jset.size() << endl;
	return 0;
}