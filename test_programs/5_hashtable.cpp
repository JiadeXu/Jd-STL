/*************************************************************************
	> File Name: 5_hashtable.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat Oct  2 00:07:25 2021
 ************************************************************************/

#include "../jd_hashtable.h"
#include "../jd_function.h"
#include <functional>
#include <iostream>
using namespace std;

int main() {
	JD::hashtable<int,
				int, 
				std::hash<int>, 
				JD::identity<int>,
				std::equal_to<int> >
		iht(50, std::hash<int>(), std::equal_to<int>());
	
	cout << iht.size() << endl;
	cout << iht.bucket_count() << endl;
	cout << iht.max_bucket_count() << endl;

	iht.insert_unique(59);
	iht.insert_unique(63);
	iht.insert_unique(108);
	iht.insert_unique(2);
	iht.insert_unique(53);
	iht.insert_unique(55);
	cout << iht.size() << endl;

	cout << "iht ";
	auto itr = iht.begin();
	for (int i = 0; i < iht.size(); ++i, ++itr) {
		cout << *itr << " ";
	}
	cout << endl;

	for (int i = 0; i < iht.bucket_count(); i++) {
		int n = iht.elems_in_bucket(i);
		if (n > 0) {
			cout << "bucket [" << i << "] has " << n << " eles" << endl;
		}
	}

	cout << "验证桶个数 与 vec大小" << endl;
	for (int i = 0; i <= 47; i++) {
		iht.insert_equal(i);
	}
	cout << "insert_equal size " << iht.size() << " bucket_count ";
	cout << iht.bucket_count() << endl;
	for (int i = 0; i < iht.bucket_count(); i++) {
		int n = iht.elems_in_bucket(i);
		if (n > 0) {
			cout << "bucket [" << i << "] has " << n << " eles" << endl;
		}
	}
	cout << "end" << endl;
	cout << endl;
	auto itr2 = iht.begin();
	for (int i = 0; i < iht.size() && itr2.cur; ++i, ++itr2) {
		cout << *itr2 << " ";
	}
	cout << endl;
	cout << *(iht.find(2)) << endl;
	cout << iht.count(2) << endl;
	return 0;
}