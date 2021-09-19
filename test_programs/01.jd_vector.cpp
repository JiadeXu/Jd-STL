#include "../jd_vector.h"
#include <vector>
#include <string>

using namespace std;

void jd_oom_handler() {
    cout << "jd oom handler" << endl;
}

int main() {
    // int arr[5] = {0, 1, 2, 3, 4};
    std::string arr[5] = {
        "as1",
        "asd2",
        "asd3",
        "asd4",
        "asd5"
    };
    // std::vector<std::string> a2(arr, arr + 5);
    // a2.erase(a2.begin(), a2.begin() + 4);
    // for (auto t : a2) {
    //     cout << t << " ";
    // }
    // return 0;
    // std::allocator<int>;
    JD::malloc_alloc::set_malloc_handler(jd_oom_handler); // 类似std::set_new_handler
    JD::vector<string> iv;
    for (int i = 0; i < 5; i++) {
        iv.push_back(arr[i]);
    }
    std::copy(iv.begin(), iv.begin() + 2, iv.begin() + 3);
    iv.insert(iv.begin() + 2, 3, string("aslkdjlas"));
    // std::sort(iv.begin(), iv.end());
    for (int i = 0 ; i < iv.size(); i++) {
        cout << iv[i] << " ";
    }
    cout << endl;

    cout << iv[3] << endl;
    cout << "erase size " << iv.size() << endl;
    iv.erase(iv.begin() + 3, iv.begin() + 6);
    for (int i = 0 ; i < iv.size(); i++) {
        cout << iv[i] << " ";
    }

    return 0;
}