#include <iostream>
#include <vector>
#include <algorithm>

const int inf = 1e9 + 7;

void counting_sort(std::vector<int> &a) {
    int mn = inf;
    int mx = -inf;
    for (auto &x : a) {
        mx = std::max(x, mx);
        mn = std::min(x, mn);
    }
    if (mn < 0) {
        for (auto &x : a) {
            x += -mn;
        }
        mx += -mn;
    }
    int n = a.size();
    std::vector<int> c(mx + 1);
    for (auto &x : a) {
        ++c[x];
    }
    for (int i = 1; i <= mx; ++i) {
        c[i] += c[i - 1];
    }
    std::vector<int> ans(n);
    for (int i = n - 1; i >= 0; --i) {
        int x = a[i];
        ans[--c[x]] = x;
    }
    
    if (mn < 0) {
        for (auto &x : ans) {
            x -= -mn;
        }
    }
    a = ans;
}

int main() {
    int n;
    std::cin >> n;

    std::vector<int> a(n);

    for (auto &x : a) {
        std::cin >> x;
    }

    counting_sort(a);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}