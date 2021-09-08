/*************************************************************************
	> File Name: lconfig-operator-overloading.cpp
	> Author: 
	> Mail: 
	> Created Time: Wed Sep  8 23:52:00 2021
 ************************************************************************/
// 自增 自减 取值
#include <iostream>
#include <ostream>
using namespace std;

class INT {
	friend ostream& operator<<(ostream &os, const INT &i);
public:
	INT(int i): m_i(i) {}
	// 前加
	INT& operator++() {
		m_i++;
		return *this;
	}
	// 后加
	INT operator++(int) {
		INT temp = *this;
		m_i++;
		return temp;
	}
	// 前减
	INT& operator--() {
		m_i--;
		return *this;
	}
	// 后加
	INT operator--(int) {
		INT temp = *this;
		m_i--;
		return temp;
	}
	int& operator*() {
		return m_i;
	}
private:
	int m_i;
};

ostream& operator<<(ostream &os, const INT &i) {
	os << '[' << i.m_i << ']';
	return os;
}

int main() {
	INT i(5);
	cout << (i++) << endl; // 5
	cout << (++i) << endl; // 7
	cout << (i--) << endl; // 7
	cout << (--i) << endl; // 5
	cout << (*i) << endl; // 5
	return 0;
}