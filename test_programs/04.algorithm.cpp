/*************************************************************************
	> File Name: 04.priority_queuue.cpp
	> Author: 
	> Mail: 
	> Created Time: Thu Sep 23 23:21:44 2021
 ************************************************************************/

#include <cstdlib>
#include <cstring>
#include <iostream>
#include "vector"
#include <algorithm>
#include "../jd_algorithm.h"
#include "../jd_vector.h"
#include "../jd_queue.h"
#include <ctime>
using namespace std;

// using namespace std;
#define SBEGIN(x) namespace x {
#define SEND(x) }

template<class ITR_T>
void display(ITR_T begin, ITR_T end) {
    for (ITR_T itr = begin; itr != end; itr++) {
        std::cout << *itr << " ";
    }
    std::cout << std::endl;
}

SBEGIN(t1)
#define MAXN 30
#define SWAP(a, b) { \
	__typeof(a) _tmp = a; \
	a = b; \
	b = _tmp; \
}

#define TEST_FUNC(arr, len, func, args...) { \
	std::cout << #func << endl; \
	int *num = new int[len]; \
	std::copy(arr, arr + len, num); \
	func(args); \
	display(num, num + MAXN); \
	delete [] num; \
}

// 从 ind 向下 最大堆化
void max_heapify(int *arr, int len, int ind) {
	while((ind << 1) <= len) {
		int tmp = ind, l = ind << 1, r = (ind << 1) + 1;
		if (arr[l] > arr[tmp]) {
			tmp = l;
		}
		if (r <= len && arr[r] > arr[tmp]) {
			tmp = r;
		}
		if (tmp == ind) break;

		SWAP(arr[tmp], arr[ind]);
		ind = tmp;
	}
}

void heap_sort(int *arr, int len) {
	arr -= 1;
	for (int i = len >> 1; i >= 1; i--) {
		max_heapify(arr, len, i);
	}
	for (int i = len; i > 1; i--) {
		SWAP(arr[i], arr[1]);
		max_heapify(arr, i - 1, 1);
	}
}

void merge_sort(int *arr, int l, int r) {
	if (r - l <= 1) {
		if (r - l == 1 && arr[l] > arr[r]) {
			SWAP(arr[r], arr[l]);
		}
		return;
	}
	int mid = l + (r - l) / 2;
	merge_sort(arr, l, mid);
	merge_sort(arr, mid + 1, r);
	int *tmp = new int[r - l + 1];
	int p1 = l, p2 = mid + 1, k = 0;
	while(p1 <= mid || p2 <= r) {
		if (p2 > r || (p1 <= mid && arr[p1] < arr[p2])) {
			tmp[k++] = arr[p1++];
		} else {
			tmp[k++] = arr[p2++];
		}
	}
	// memcpy(arr + l, tmp, sizeof(int) * (r - l + 1));
	for (int i = 0; i < k; i++) {
		arr[l + i] = tmp[i];
	}
	delete[] tmp;
}

void quick_sort(int *arr, int l, int r) {
	if (l > r) return;
	// z是基准
	int x = l, y = r, z = arr[x];
	while(x < y) {
		while(x < y && arr[y] > z) {
			y--;
		}
		if (x < y) {
			arr[x++] = arr[y];
		}
		while(x < y && arr[x] < z) {
			x++;
		}
		if (x < y) {
			arr[y--] = arr[x];
		}
	}
	arr[x] = z;
	// cout << "tmp ";
	// display(arr + l, arr + r + 1);
	quick_sort(arr, l, x - 1);
	quick_sort(arr, x + 1, r);
}

int main() {
	srand(time(nullptr));

	int *arr = new int[MAXN];
	for (int i = 0; i < MAXN; i++) {
		arr[i] = rand() % 100;
		cout << arr[i] << " ";
	}
	cout << endl;
	// TEST_FUNC(heap_sort, arr, MAXN);
	TEST_FUNC(arr, MAXN, heap_sort, num, MAXN);
	TEST_FUNC(arr, MAXN, merge_sort, num, 0, MAXN - 1);
	TEST_FUNC(arr, MAXN, quick_sort, num, 0, MAXN - 1);
	JD::vector<int> ivec;
	for (int i = 0; i < MAXN; i++) {
		ivec.push_back(arr[i]);
	}
	JD::make_heap(ivec.begin(), ivec.end());
	JD::sort_heap(ivec.begin(), ivec.end());
	display(ivec.begin(), ivec.end());
	delete [] arr;
	return 0;
}
#undef MAXN
SEND(t1)

SBEGIN(t2)
int main() {
	std::cout << "STL heap" << std::endl;
	int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
	std::vector<int> ivec;
	for (int i = 0; i < 9; i++) {
		ivec.push_back(ia[i]);
	}
	display(ivec.begin(), ivec.end());
	std::make_heap(ivec.begin(), ivec.end());
	display(ivec.begin(), ivec.end());

	ivec.push_back(7);
	std::push_heap(ivec.begin(), ivec.end());
	display(ivec.begin(), ivec.end());
	
	std::pop_heap(ivec.begin(), ivec.end());
	std::cout << ivec.back() << std::endl; // 输出的是9 但并未被弹出
	ivec.pop_back();
	display(ivec.begin(), ivec.end());

	std::sort_heap(ivec.begin(), ivec.end());
	display(ivec.begin(), ivec.end());
	return 0;
}
SEND(t2)

SBEGIN(t3)
int main() {
	std::cout << "JD heap" << std::endl;
	int ia[9] = {0, 1, 2, 3, 4, 8, 9, 3, 5};
	JD::vector<int> ivec;
	for (int i = 0; i < 9; i++) {
		ivec.push_back(ia[i]);
	}
	display(ivec.begin(), ivec.end());
	auto comp = JD::JDGreat<int>(); // 小顶堆
	// auto comp = JD::JDLess<int>(); // 大顶堆
	// auto comp = [](const int &a, const int &b) {
	// 	return a < b; // 大顶堆
	// };
	JD::make_heap(ivec.begin(), ivec.end());
	display(ivec.begin(), ivec.end());

	ivec.push_back(7);
	JD::push_heap(ivec.begin(), ivec.end());
	display(ivec.begin(), ivec.end());
	
	JD::pop_heap(ivec.begin(), ivec.end());
	std::cout << ivec.back() << std::endl; // 输出的是9 但并未被弹出
	ivec.pop_back();
	display(ivec.begin(), ivec.end());

	JD::sort_heap(ivec.begin(), ivec.end());
	display(ivec.begin(), ivec.end());

	// JD::priority_queue<int> myp; // 默认是大顶堆
	JD::priority_queue<int, JD::vector<int>, JD::JDGreat<int> > myp; // 小顶堆
	for (int i = 0; i < 9; i++) {
		myp.push(ia[i]);
	}
	std::cout << "jd priority queue: ";
	while(!myp.empty()) {
		std::cout << myp.top() << " ";
		myp.pop();
	}
	std::cout << std::endl;
	return 0;
}
SEND(t3)

int main() {
	t1::main();
	t2::main();
	t3::main();
	return 0;
}