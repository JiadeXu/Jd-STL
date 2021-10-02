/*************************************************************************
	> File Name: o5_test_hashfunc.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Oct  2 14:12:11 2021
 ************************************************************************/

#include<iostream>
#include <sys/_types/_size_t.h>
#include "../jd_hashtable.h"
#include "../jd_function.h"
#include "../jd_hash_fun.h"
using namespace std;

struct hash_str {
	size_t operator()(const string &str) const {
		return JD::_jd_hash_string(str.c_str());
	}
};

int main() {
	JD::hashtable<string,
			string, 
			hash_str, 
			JD::identity<string>,
			std::equal_to<string> >
	iht(50, hash_str(), std::equal_to<string>());
	cout << iht.size() << endl;
	cout << iht.bucket_count() << endl;
	iht.insert_unique(string("XJD"));
	iht.insert_unique(string("JADE"));
	for (auto it : iht) {
		cout << it << " ";
	}
	cout << endl;
	return 0;
}