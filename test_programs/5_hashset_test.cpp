/*************************************************************************
	> File Name: 5haset_test.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Oct  2 20:46:39 2021
 ************************************************************************/

#include<iostream>
#include "../jd_hash_set.h"
#include <cstring>
#include <ostream>
using namespace std;

struct eqstr {
	bool operator()(const char *s1, const char *s2) {
		return strcmp(s1, s2) == 0;
	}
};

void lookup(JD::hash_set<const char *, JD::my_hash<const char *>, eqstr> &set, const char *word) {
	auto it = set.find(word);
	cout << " " << word << " " << (it != set.end() ? "present" : "not present") << endl;
}

int main() {
	JD::hash_set<const char *, JD::my_hash<const char *>, eqstr> my_set;
	my_set.insert("kiwi");
	my_set.insert("plum");
	my_set.insert("apple");
	my_set.insert("mango");
	my_set.insert("apricot");
	my_set.insert("banana");

	lookup(my_set, "mango");
	lookup(my_set, "apple");
	lookup(my_set, "durian");

	for (auto it = my_set.begin(); it != my_set.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;

	JD::hash_set<int> int_set;
	int_set.insert(59);
	int_set.insert(63);
	int_set.insert(108);
	int_set.insert(2);
	int_set.insert(53);
	int_set.insert(55);
	
	for (auto it = int_set.begin(); it != int_set.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	return 0;
}