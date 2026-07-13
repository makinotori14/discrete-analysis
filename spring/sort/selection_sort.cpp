#include <iostream>
#include <vector>
#include <algorithm>

void selection_sort(std::vector<int> &a) {
    for (int i = 0; i < a.size(); ++i) {
        int mn = i;
        for (int j = i; j < a.size(); ++j) {
            if (a[mn] > a[j]) {
                mn = j;
            }
        }
        std::swap(a[i], a[mn]);
    }
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);

    for (auto &x : a) {
        std::cin >> x;
    }

    selection_sort(a);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}