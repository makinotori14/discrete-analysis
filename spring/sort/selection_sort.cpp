#include <iostream>
#include <vector>
#include <algorithm>

void selection_sort(std::vector<int> &a) {
    std::vector<int> ids(a.size());
    for (int i = 0; i < a.size(); ++i) {
        ids[i] = i + 1;
    }
    for (int i = 0; i < a.size(); ++i) {
        int mn = i;
        for (int j = i + 1; j < a.size(); ++j) {
            std::cout << "a_" << ids[j] << " : " << "a_" << ids[mn] << '\n';
            if (a[mn] > a[j]) {
                std::cout << "a_" << ids[j] << " < " << "a_" << ids[mn] << '\n';
                mn = j;
            } else {
                std::cout << "a_" << ids[j] << " >= " << "a_" << ids[mn] << '\n';
            }
        }
        std::swap(a[i], a[mn]);
        std::swap(ids[i], ids[mn]);
    }

    for (auto &x : ids) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
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