#include <iostream>
#include <iterator>
#include "jd_map.h"
#include "jd_set.h"
#include "jd_iterator.h"

using namespace std;

#define BEGIN(x) namespace x {
#define ENDS(x) }

BEGIN(test_itr)
int main() {
    int arr[3] = {1, 2, 3};
    // JD::advance(arr, 2);
    int *p = arr;
    cout << *p << endl;
    // advance(arr, 2);
    JD::advance(p, 2);
    cout << *p << endl;
    int *a = arr;
    cout << "distance " << JD::distance(a, p) << endl;
    cout << "distance " << distance(a, p) << endl;
    return 0;
}
ENDS(test_itr)

BEGIN(map_set)
auto func = [](int a) {
    cout << "auto a " << a << endl;
};

int main() {
    func(233);
    JD::set<int> tree;
    srand(time(0));
    for (int i = 0; i < 23; i++) {
        int val = rand() % 100;
        cout << val << " ";
        tree.insert(val);
    }
    cout << endl;
    cout << "a" << endl;
    for (auto itr : tree) {
        cout << itr << " ";
    }
    cout << endl;
    cout << "set endl" << endl;
    // tree.output();
    // cout << "set end --------" << endl;
    JD::map<int, int> map;
    cout << "insert" << endl;
    for (int i = 0 ; i < 13; i++) {
        int key = rand() % 100;
        int val = rand() % 100;
        cout << "(" << key << ", " << val << ")" << endl;
        map[key] = val;
    }
    cout << "for each" << endl;
    map.output();
    for (auto itr : map) {
        cout << itr.first << " " << itr.second << endl;
    }
    for (auto i = map.begin(); i != map.end(); i++) {
        cout << (*i).first << " " << (*i).second << endl;
    }
    cout << endl;
    return 0;
}

ENDS(map_set)

int main() {
    test_itr::main();
    return 0;
}