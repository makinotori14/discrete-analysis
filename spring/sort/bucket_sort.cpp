#include <iostream>
#include <vector>
#include <algorithm>

double inf = 1e9 + 7;

void insertion_sort(std::vector<double> &a) {
    int n = a.size();
    for (int i = 1; i < n; ++i) {
        double x = a[i];

        int j = i - 1;
        while (j >= 0 && a[j] > x) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = x;
    }
}

void bucket_sort(std::vector<double> &a) {
    double mn = inf;
    double mx = -inf;
    for (auto &x : a) {
        mn = std::min(mn, x);
        mx = std::max(mx, x);
    }
    double range = mx - mn;
    int n = a.size();
    std::vector<std::vector<double>> v(n);
    for (auto &x : a) {
        int index = int(((x - mn) * 1.0 * n) / range);
        if (index == n) {
            --index;
        }
        v[index].push_back(x);
    }

    for (int i = 0; i < n; ++i) {
        insertion_sort(v[i]);
    }
    int cur = 0;
    std::vector<double> ans(n);
    for (int i = 0; i < n; ++i) {
        for (auto &x : v[i]) {
            ans[cur++] = x;
        }
    }
    a = ans;
}

int main() {
    #ifdef LOCAL
        freopen("input.txt", "r", stdin);
    #endif

    int n;
    std::cin >> n;
    std::vector<double> a(n);
    for (auto &x : a) {
        std::cin >> x;
    }
    // Чтобы сортировка работала за линейное время, нужно, чтобы элементы были равномерно распределены
    bucket_sort(a);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}
