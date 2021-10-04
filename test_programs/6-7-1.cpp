/*************************************************************************
	> File Name: 6-7-1.cpp
	> Author: 
	> Mail: 
	> Created Time: Sun Oct  3 23:44:53 2021
 ************************************************************************/

#include <algorithm>
#include <functional>
#include <iostream>
#include "../jd_vector.h"
#include <vector>
using namespace std;

template<class T>
struct display {
	void operator()(const T &x) const {
		cout << x << " ";
	}
};

struct even {
	bool operator()(int x) const {
		return x % 2 ? false : true;
	}
};

class even_by_two {
public:
	int operator()() const {
		return _x += 2;
	}
private:
	static int _x;
};
int even_by_two::_x = 0;

int main() {
	int ia[] = {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
	std::vector<int> iv(ia, ia + sizeof(ia) / sizeof(int));

	// 找出 iv 之中相邻元素值相等的第一个元素
	cout << *JD::adjacent_find(iv.begin(), iv.end(), std::equal_to<int>()) << endl; // 6

	// iv 之中相邻元素值相等的第一个元素
	cout << "with equal " << *JD::adjacent_find(iv.begin(), iv.end()) << endl; // 6
	
	// 找出iv之中元素值为6的元素个数
	cout << "count equal 6: " << JD::count(iv.begin(), iv.end(), 6) << endl;

	// auto f = [](int i) {
	// 	return i < 7;
	// };
	// cout << "count less than 7: " << JD::count_if(iv.begin(), iv.end(), f) << endl;
	cout << "count less than 7: " << JD::count_if(iv.begin(), iv.end(), bind(std::less<int>(), placeholders::_1, 7)) << endl;

	// 找出iv 之中元素值为4的第一个元素所在位置的值
	cout << "find 4 " << *JD::find(iv.begin(), iv.end(), 4) << endl;
	cout << "find_if greater 2 " << *JD::find_if(iv.begin(), iv.end(), bind(std::greater<int>(), placeholders::_1, 2)) << endl;

	// 找出iv中iv2子序列出现的最后一个位置
	std::vector<int> iv2(ia + 6, ia + 8);
	cout << "find_end " << *(JD::find_end(iv.begin(), iv.end(), iv2.begin(), iv2.end()) + 3) << endl;
	cout << "find_fitst_of " << *(JD::find_first_of(iv.begin(), iv.end(), iv2.begin(), iv2.end()) + 3) << endl;
	cout << "generate" << endl;
	JD::generate(iv2.begin(), iv2.end(), even_by_two());
	cout << "gen iv2: ";
	JD::for_each(iv2.begin(), iv2.end(), display<int>());
	cout << endl; // 2 4

	cout << "gen_n iv1: ";
	JD::generate_n(iv.begin(), 3, even_by_two());
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; // 6 8 10 3 4 5 6 6 6 7 8

	cout << "remove block" << endl;
	auto a = JD::remove(iv.begin(), iv.end(), 6);
	cout << "remove iv: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; // 8 10 3 4 5 7 8 7 8 0 0 0

	std::vector<int> iv3(12);
	JD::remove_copy(iv.begin(), iv.end(), iv3.begin(), 6);
	cout << "remove_copy iv3: ";
	JD::for_each(iv3.begin(), iv3.end(), display<int>());
	cout << endl; // 8 10 3 4 5 7 8 7 8 0 0 0

	cout << "remove_if iv: ";
	auto b = JD::remove_if(iv.begin(), iv.end(), bind(std::less<int>(), placeholders::_1, 6));
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; //  8 10 7 8 6 6 7 8 6 7 8

	cout << "remove_copy_if iv3: ";
	JD::remove_copy_if(iv.begin(), iv.end(), iv3.begin(), bind(std::less<int>(), placeholders::_1, 7));
	JD::for_each(iv3.begin(), iv3.end(), display<int>());
	cout << endl; //  8 10 7 8 7 8 8 7 8 0 0 0
	
	cout << "replace block" << endl;
	
	JD::replace(iv.begin(), iv.end(), 6, 3);
	cout << "replace iv2 6 -> 3: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; //  8 10 7 8 3 3 7 8 3 7 8

	JD::replace_copy(iv.begin(), iv.end(), iv3.begin(), 3, 5);
	cout << "replace_copy iv3 3 -> 5: ";
	JD::for_each(iv3.begin(), iv3.end(), display<int>());
	cout << endl; //  8 10 7 8 5 5 7 8 5 7 8 0

	JD::replace_if(iv.begin(), iv.end(), bind(std::less<int>(), placeholders::_1, 5), 2);
	cout << "replace_if iv3 [less then 5] -> 2: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; //  8 10 7 8 2 2 7 8 2 7 8

	JD::replace_copy_if(iv.begin(), iv.end(), iv3.begin(), bind(std::equal_to<int>(), placeholders::_1, 8), 9);
	cout << "replace_copy_if iv3 [equal to 8] -> 9: ";
	JD::for_each(iv3.begin(), iv3.end(), display<int>());
	cout << endl; //  9 10 7 9 2 2 7 9 2 7 9 0

	cout << "reverse block" << endl;
	JD::reverse(iv.begin(), iv.end());
	cout << "reverse iv: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; //  8 7 2 8 7 2 2 8 7 10 8
	
	JD::reverse_copy(iv.begin(), iv.end(), iv3.begin());
	cout << "reverse iv: ";
	JD::for_each(iv3.begin(), iv3.end(), display<int>());
	cout << endl; //  8 10 7 8 2 2 7 8 2 7 8 0

	cout << "rotate" << endl;
	JD::rotate(iv.begin(), iv.begin() + 4, iv.end());
	cout << "rotate iv: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; //  7 2 2 8 7 10 8 8 7 2 8

	
	JD::rotate_copy(iv.begin(), iv.begin() + 5, iv.end(), iv3.begin());
	cout << "rotate copy iv: ";
	JD::for_each(iv3.begin(), iv3.end(), display<int>());
	cout << endl; // 10 8 8 7 2 8 7 2 2 8 7 0

	// 查找某个子序列第一次出现地点
	int ia2[3] = {2, 8};
	std::vector<int> iv4(ia2, ia2 + 2);
	cout << "search 2 8 in iv: " << *(JD::search(iv.begin(), iv.end(), iv4.begin(), iv4.end())) << endl; // 2
	// 查找乱序出现2 个 8 子序列起点
	cout << "search_n 2 8: " << *JD::search_n(iv.begin(), iv.end(), 2, 8) << endl; // 8

	// 将两个区间内的元素互换 第二区间的元素个数不应该小于第一区间元素个数
	JD::swap_ranges(iv4.begin(), iv4.end(), iv.begin());
	cout << "swap_ranges iv: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; // 2 8 2 8 7 10 8 8 7 2 8
	cout << "swap_ranges iv4: ";
	JD::for_each(iv4.begin(), iv4.end(), display<int>());
	cout << endl; // 7 2

	// auto f = JD::minus<int>();
	// f()
	JD::transform(iv.begin(), iv.end(), iv.begin(), bind(std::minus<int>(), placeholders::_1, 2));
	cout << "transform minus 2 iv: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; // 0 6 0 6 5 8 6 6 5 0 6

	JD::transform(iv.begin(), iv.end(), iv.begin(), iv.end(), std::plus<int>());
	cout << "transform minus range to range iv: ";
	JD::for_each(iv.begin(), iv.end(), display<int>());
	cout << endl; // 0 12 0 12 10 16 12 12 10 0 12

	cout << "\nvec\n" << endl;
	std::vector<int> iv7(15);
	std::vector<int> iv5(ia, ia + sizeof(ia) / sizeof(int));
	std::vector<int> iv6(ia + 4, ia + 8);
	cout << "iv5: ";
	JD::for_each(iv5.begin(), iv5.end(), display<int>());
	cout << endl; // 0 1 2 3 4 5 6 6 6 7 8
	JD::for_each(iv6.begin(), iv6.end(), display<int>());
	cout << endl; // 4 5 6 6
	
	cout << *JD::max_element(iv5.begin(), iv5.end()) << endl;
	cout << *JD::min_element(iv5.begin(), iv5.end()) << endl;

	cout << "iv5 include 6 is : " << JD::includes(iv5.begin(), iv5.end(), iv6.begin(), iv6.end()) << endl; // 1

	// 两个序列必须是有序的
	cout << "merge ";
	JD::merge(iv5.begin(), iv5.end(), iv6.begin(), iv6.end(), iv7.begin());
	JD::for_each(iv7.begin(), iv7.end(), display<int>());
	cout << endl; // 0 1 2 3 4 4 5 5 6 6 6 6 6 7 8

	// 符合条件的元素放在容器前端 不符合放后段
	// 非stable
	JD::partition(iv7.begin(), iv7.end(), even());
	cout << "iv7 partition: ";
	JD::for_each(iv7.begin(), iv7.end(), display<int>());
	cout << endl; // 4 5 6 6

	// 去除连续而重复的元素
	auto c = JD::unique(iv5.begin(), iv5.end());
	cout << "unique iv5: ";
	JD::for_each(iv5.begin(), iv5.end(), display<int>());
	cout << endl; // 0 1 2 3 4 5 6 7 8 7 8


	auto d = JD::unique_copy(iv5.begin(), iv5.end(), iv7.begin());
	cout << "unique_copy iv5: ";
	JD::for_each(iv7.begin(), iv7.end(), display<int>());
	cout << endl; // 0 1 2 3 4 5 6 7 8 7 8 3 7 1
	return 0;
}