/*************************************************************************
	> File Name: 05_map_test.cpp
	> Author: 
	> Mail: 
	> Created Time: Fri Oct  1 09:50:44 2021
 ************************************************************************/

#include <iostream>
#include <map>
#include <string>
#include "../jd_map.h"
using namespace std;

int main() {
	JD::map<string, int> simap;
	simap[string("jjhou")] = 1;
	simap[string("jerry")] = 2;
	simap[string("jason")] = 3;
	simap[string("jlmmy")] = 4;

	std::pair<string, int> value(string("david"), 5);
	simap.insert(value);
	cout << simap.size() << endl;

	cout << "david " << simap["david"] << endl;

	JD::map<string, int>::iterator simap_itr = simap.begin();
	for (; simap_itr != simap.end(); simap_itr++) {
		cout << "map " << simap_itr->first << " " << simap_itr->second << endl;
	}
	cout << "jjhou is " << simap["jjhou"] << endl;
	JD::map<string, int>::iterator itr1;

	itr1 = simap.find("jason");
	if (itr1 == simap.end()) {
		cout << "Not found jason" << endl;
	}
	itr1 = simap.find("jx");
	if (itr1 == simap.end()) {
		cout << "Not found jx" << endl;
	}
	simap["jjhou"] = 77889;
	cout << "jjhou 2 is " << simap["jjhou"] << endl;
	cout << "cout s22 : " << endl;
	simap_itr = simap.begin();
	for (; simap_itr != simap.end(); simap_itr++) {
		cout << simap_itr->first << ": " << simap_itr->second << "; ";
	}
	cout << endl;
	return 0;
}