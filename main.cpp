#include <iostream>
#include "jd_map.h"
#include "jd_set.h"

using namespace std;

auto func = [](int a) {
    cout << "auto a " << a << endl;
};

int main() {
    func(233);
    Jd::set<int> tree;
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
    Jd::map<int, int> map;
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
