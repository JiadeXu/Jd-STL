#include <iostream>
#include <iterator>
using namespace std;

struct B {};
struct D1 : public B {};
struct D2 : public D1 {};

template<class I>
void func(I &a, B) {
    cout << "B Version" << endl;
}

template<class I>
void func(I &a, D2) {
    cout << "D2 Version" << endl;
}

int main() {
    int *p;
    cout << "B() "; func(p, B()); // B V
    cout << "D1() "; func(p, D1()); // 输出 B V
    cout << "D2() "; func(p, D2()); // D2 V
    return 0;
}