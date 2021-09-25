#include <list>
#include <iostream>
#include <algorithm>
#include "../jd_list.h"
#include "../jd_slist.h"
#include <ctime>

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

template<class ITR_T>
void reverseDisplay(ITR_T begin, ITR_T end) {
    ITR_T itr = end;
    for (;;) {
        if (itr == begin) {
            std::cout << *itr;
            break;
        }
        if (itr == end) {
            itr--;
            continue;
        }
        std::cout << *itr << ' ';
        itr--;
    } 
    std::cout << std::endl;
}

SBEGIN(t1)
int main() {
    std::list<int> ilist;
    std::cout << "init" << std::endl;
    std::cout << "list size " << ilist.size() << std::endl;

    for (int i = 0 ; i < 5; i++)
        ilist.push_back(i);
    std::cout << "list size " << ilist.size() << std::endl;
    
    std::list<int>::iterator itr;
    for (itr = ilist.begin(); itr != ilist.end(); itr++) {
        std::cout << *itr << " ";
    }
    std::cout << std::endl;

    itr = std::find(ilist.begin(), ilist.end(), 3);
    if (itr != ilist.end()) {
        ilist.insert(itr, 99);
    }
    std::cout << "after insert" << std::endl;
    std::cout << "list size " << ilist.size() << std::endl;
    std::cout << *itr << std::endl; // 3

    for (itr = ilist.begin(); itr != ilist.end(); itr++) {
        std::cout << *itr << " ";
    }
    std::cout << std::endl;

    itr = std::find(ilist.begin(), ilist.end(), 1);
    if (itr != ilist.end()) {
        std::cout << "cur erase " << *ilist.erase(itr) << std::endl; // 删除了 1 返回的是 2
    }
    std::cout << "after erase" << std::endl;
    std::cout << "list size " << ilist.size() << std::endl;
    std::cout << *itr << std::endl; // 1

    for (itr = ilist.begin(); itr != ilist.end(); itr++) {
        std::cout << *itr << " ";
    }
    std::cout << std::endl;
    // ilist.swap(list<int> &__c)
    std::cout << "swap" << std::endl;
    std::list<int> il2;
    srand(time(nullptr));
    for (int i = 0; i < 10; i++) {
        il2.push_back(rand() % 100);
    }
    display(ilist.begin(), ilist.end());
    display(il2.begin(), il2.end());
    ilist.swap(il2);
    display(ilist.begin(), ilist.end());
    display(il2.begin(), il2.end());
    return 0;
}
SEND(t1)

SBEGIN(t2)
int main() {

    JD::list<int> ilist;
    std::cout << "init" << std::endl;
    std::cout << "list size " << ilist.size() << std::endl;
    for (int i = 0 ; i < 5; i++)
        ilist.push_back(i);

    JD::list<int>::iterator itr = ilist.begin();
    display(ilist.begin(), ilist.end());
    reverseDisplay(ilist.begin(), ilist.end());

    std::cout << "list size " << ilist.size() << std::endl;
    itr = ilist.insert(itr + 2, 99);
    std::cout << *itr << std::endl; // 3
    std::cout << "after insert" << std::endl;
    std::cout << "list size " << ilist.size() << std::endl;
    display(ilist.begin(), ilist.end());

    itr = itr + 1;
    std::cout << "erase " << *itr << " " << *ilist.erase(itr);
    std::cout << " after erase; size " << ilist.size() << std::endl;
    ilist.pop_front();
    display(ilist.begin(), ilist.end());
    ilist.clear();
    for (int i = 0; i < 5; i++) {
        ilist.push_front(12);
    }
    for (int i = 0; i < 5; i++) {
        ilist.push_back(11);
    }
    itr = ilist.begin() + 3;
    std::cout << "start splice" << std::endl;
    display(ilist.begin(), ilist.end());
    ilist.splice(ilist.begin(), itr, ilist.end());
    std::cout << "after splice ";
    display(ilist.begin(), ilist.end());
    ilist.unique();
    display(ilist.begin(), ilist.end());
    ilist.remove(12);
    display(ilist.begin(), ilist.end());
    std::cout << ilist.size() << std::endl;

    ilist.clear();
    std::cout << "clear " << ilist.size() << std::endl;

    srand(time(nullptr));
    for (int i = 0; i < 10; i++) {
        ilist.push_back(rand() % 100);
    }
    display(ilist.begin(), ilist.end());
    ilist.reverse();
    display(ilist.begin(), ilist.end());


    std::cout << "JD swap" << std::endl;
    JD::list<int> il2;
    srand(time(nullptr));
    for (int i = 0; i < 10; i++) {
        il2.push_back(rand() % 100);
    }
    display(ilist.begin(), ilist.end());
    display(il2.begin(), il2.end());
    ilist.swap(il2);
    display(ilist.begin(), ilist.end());
    il2.sort();
    std::cout << "sort reverse ";
    il2.reverse();
    display(il2.begin(), il2.end());

    return 0;
}
SEND(t2)

SBEGIN(slist_test)
int main() {

    JD::slist<int> s;
    s.push_front(233);
    s.push_front(6);
    s.push_front(7);
    s.push_front(4);
    s.push_front(8);
    std::cout << s.size() << std::endl;
    JD::slist<int>::iterator ite = s.begin();
    JD::slist<int>::iterator ite2 = s.end();
    for (; ite != ite2; ite++) {
        std::cout << *ite << " ";
    }
    std::cout << std::endl;;

    ite = s.begin();
    s.insert(ite + 2, 99);
    s.insert(s.begin(), 34);
    ite = s.begin();
    ite2 = s.end();
    for (; ite != ite2; ++ite) {
        std::cout << *ite << " ";
    }
    std::cout << std::endl;;

    JD::slist<int>::iterator toDelete = s.begin() + 3;
    std::cout << "toDelete " << *toDelete << " " << *(s.erase(toDelete)) << std::endl;
    ite = s.begin();
    ite2 = s.end();
    for (; ite != ite2; ++ite) {
        std::cout << *ite << " ";
    }
    std::cout << std::endl;;
    return 0;
}
SEND(slist_test)

int main() {
    // t1::main();
    // t2::main();
    slist_test::main();
    return 0;
}