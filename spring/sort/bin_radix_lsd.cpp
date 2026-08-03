#include <iostream>
#include <vector>
#include <stdint.h>

const int B = 32;

int getLog2(int x) {
    int k = 0;
    while ((1ll << k) <= x) {
        ++k;
    }
    return k - 1;
}

uint32_t mask(uint32_t x, int k, int sz) {
    uint32_t tr = (1ull << (sz * (k + 1))) - 1;
    return (x & tr) >> (sz * k);
}

void counting_sort(std::vector<uint32_t> &a, int r, int iter) {
    int n = a.size();
    int mx = (1 << r);
    std::vector<int> c(mx);
    for (int i = 0; i < n; ++i) {
        int x = mask(a[i], iter, r);
        ++c[x];
    }
    for (int i = 1; i < mx; ++i) {
        c[i] += c[i - 1];
    }
    std::vector<uint32_t> ans(n);
    for (int i = n - 1; i >= 0; --i) {
        int x = mask(a[i], iter, r);
        ans[--c[x]] = a[i];
    }
    a = ans;
}

void bin_radix_sort(std::vector<uint32_t> &a, int r) {
    for (int i = 0; i < (B + r - 1) / r; ++i) {
        counting_sort(a, r, i);
    }
}

int main() {
    #ifdef LOCAL
        freopen("input.txt", "r", stdin);
    #endif
    int n;
    std::cin >> n;
    std::vector<uint32_t> a(n);
    for (auto &x : a) {
        std::cin >> x;
    }
    int r = 0;
    int log_n = getLog2(n);
    if (B < log_n) {
        r = B;
    } else {
        r = log_n;
    }
    bin_radix_sort(a, r);
    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}
