#include <iostream>
#include <stdint.h>
#include <vector>

const int B = 32;
int R = 0;

int log2(int x) {
    int k = 0;
    while ((1 << k) <= x) {
        ++k;
    }
    return --k;
}

uint32_t mask(uint64_t x, int iter) {
    int sz = R;
    uint64_t tr = (1ll << (sz * (iter + 1))) - 1;
    x &= tr;
    uint32_t ans = x >> (sz * iter);
    return ans;
}

void counting_sort(std::vector<uint32_t> &a, int l, int r, int iter) {
    std::vector<uint32_t> b(r - l);
    for (int i = l; i < r; ++i) {
        b[i - l] = a[i];
    }
    int mx = 1 << R;
    std::vector<int> c(mx);
    for (int i = 0; i < r - l; ++i) {
        int x = mask(b[i], iter);
        ++c[x];
    }
    for (int i = 1; i < mx; ++i) {
        c[i] += c[i - 1];
    }
    std::vector<uint32_t> ans(r - l);
    for (int i = r - l - 1; i >= 0; --i) {
        int x = mask(b[i], iter);
        ans[--c[x]] = b[i];
    }
    for (int i = 0; i < r - l; ++i) {
        a[l + i] = ans[i];
    }
}

void radix_sort(std::vector<uint32_t> &a, int l, int r, int iter) {
    if (r - l == 1 || iter == -1) {
        return;
    }
    counting_sort(a, l, r, iter);
    int i = l;
    int j = i;
    while (j < r) {
        while (j < r && mask(a[j], iter) == mask(a[i], iter)) {
            ++j;
        }
        radix_sort(a, i, j, iter - 1);
        i = j;
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

    if (B < log2(n)) {
        R = B;
    } else {
        R = log2(n);
    }

    radix_sort(a, 0, n, (B + R - 1) / R);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}
