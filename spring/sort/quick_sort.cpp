#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>

std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());

void quick_sort(std::vector<int> &a, int l, int r) {
    if (r - l <= 1) {
        return;
    }
    std::uniform_int_distribution<int> rand(l, r - 1);
    int pivot = rand(gen);
    int x = a[pivot];
    
    int i = l;
    int j = r - 1;

    while (i <= j) {
        while (a[i] < x) {
            ++i;
        }
        while (a[j] > x) {
            --j;
        }
        if (i > j) {
            break;
        }
        std::swap(a[i], a[j]);
        ++i;
        --j;
    }
    quick_sort(a, l, j + 1);
    quick_sort(a, i, r);
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &x : a) {
        std::cin >> x;
    }

    quick_sort(a, 0, n);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}