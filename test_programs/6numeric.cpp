/*************************************************************************
	> File Name: 6numeric.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 00:18:25 2021
 ************************************************************************/

#include <iostream>
#include <numeric>
#include "../jd_vector.h"
#include "../jd_numeric.h"
#include <utility>
#include <vector>
#include <functional>
#include <iterator> // ostream_iterator
using namespace std;

int main() {
	int ia[5] = {1, 2, 3 ,4 ,5};
	JD::vector<int> iv;
	for (int i = 0; i < 5; i++) {
		iv.push_back(ia[i]);
	}

	cout << JD::accumulate(iv.begin(), iv.end(), 0) << endl;
	cout << JD::accumulate(iv.begin(), iv.end(), 0, minus<int>()) << endl;

	// 10 + 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4 + 5 * 5
	cout << JD::inner_product(iv.begin(), iv.end(), iv.begin(), iv.end(), 10) << endl;
		// 10 + 1 * 1 + 2 * 2 + 3 * 3 + 4 * 4 + 5 * 5
	cout << JD::inner_product(iv.begin(), iv.end(), iv.begin(), iv.end(), 10, minus<int>(), plus<int>()) << endl;

	ostream_iterator<int> oite(cout, " ");
	JD::partial_sum(iv.begin(), iv.end(), oite); // 1 3 6 10 15
	cout << endl;
	JD::partial_sum(iv.begin(), iv.end(), oite, minus<int>()); // 1 -1 -4 -8 -13
	cout << endl;

	JD::adjacent_difference(iv.begin(), iv.end(), oite);

	JD::adjacent_difference(iv.begin(), iv.end(), oite, plus<int>());
	cout << endl;

	cout << JD::power(10, 3) << endl;
	cout << JD::power(10, 3, std::plus<int>()) << endl; // 10 + 10 + 10

	int n = 3;
	cout << "iota ";
	JD::iota(iv.begin(), iv.end(), n);
	for (int i = 0; i < iv.size(); i++) {
		cout << "iv[" << i << "] " << iv[i] << " ";
	}
	cout << "iv[i] end " << std::endl;
	return 0;
}