#include <iostream>
#include <vector>
#include <random>
#include <chrono>

std::mt19937 gen(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()));

int rand_pivot(int l, int r) {
    int ans = std::uniform_int_distribution<int>(l, r)(gen);
    return ans;
}

void quick_sort(std::vector<int> &a, int l, int r) {
    if (r - l <= 1) {
        return;
    }
    int pivot = rand_pivot(l, r - 1);
    int v = a[pivot];

    int i = l;
    int j = r - 1;
    while (i <= j) {
        while (a[i] < v) {
            ++i;
        }
        while (a[j] > v) {
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