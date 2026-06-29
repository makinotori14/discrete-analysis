#include <iostream>
#include <vector>

void merge_sort_impl(std::vector<int> &a, std::vector<int> &temp, int l, int r) {
    if (r - l <= 1) {
        return;
    }
    int mid = (l + r) / 2;
    merge_sort_impl(a, temp, l, mid);
    merge_sort_impl(a, temp, mid, r);
    int i = l;
    int j = mid;
    int cur = l;
    while (i < mid && j < r) {
        if (a[i] <= a[j]) {
            temp[cur++] = a[i++];
        } else {
            temp[cur++] = a[j++];
        }
    }
    while (i < mid) {
        temp[cur++] = a[i++];
    }
    while (j < r) {
        temp[cur++] = a[j++];
    }
    for (int i = l; i < r; ++i) {
        a[i] = temp[i];
    }
    return;
}

void merge_sort(std::vector<int> &a) {
    std::vector<int> temp(a.size());
    merge_sort_impl(a, temp, 0, a.size());
    return;
}

int main() {
    int n;
    std::cin >> n;

    std::vector<int> a(n);
    for (auto &x : a) {
        std::cin >> x;
    }

    merge_sort(a);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}