#include <iostream>
#include <vector>
#include <algorithm>

// обменная сортировка, сортировка пузырьком

void bubble_sort(std::vector<int> &a) {
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (a[j + 1] < a[j]) {
                std::swap(a[j + 1], a[j]);
            }
        }
    }
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);

    for (auto &x : a) {
        std::cin >> x;
    }

    bubble_sort(a);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}