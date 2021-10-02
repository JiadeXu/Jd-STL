/*************************************************************************
	> File Name: 5_hashmap_test.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Oct  2 21:35:12 2021
 ************************************************************************/

#include<iostream>
#include <ostream>
#include "../jd_hash_map.h"
#include "../jd_hash_fun.h"
using namespace std;

struct eqstr {
	bool operator()(const char *s1, const char *s2) {
		return strcmp(s1, s2) == 0;
	}
};

int main() {
	JD::hash_map<const char *, int, JD::my_hash<const char *>, eqstr> days;
	days["january"] = 31;
	days["february"] = 28;
	days["march"] = 31;
	days["april"] = 30;
	days["may"] = 31;
	days["june"] = 30;
	days["july"] = 31;
	days["august"] = 31;
	days["september"] = 30;
	days["october"] = 31;
	days["november"] = 30;
	days["december"] = 31;

	cout << "September " << days["september"] << endl;
	cout << "June      " << days["june"] << endl;
	cout << "February  " << days["february"] << endl;
	cout << "December  " << days["december"] << endl;

	for (auto it = days.begin(); it != days.end(); it++) {
		cout << it->first << " ";
	}
	cout << endl;
	return 0;
}
