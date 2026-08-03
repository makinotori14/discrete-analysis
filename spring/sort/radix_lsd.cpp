#include <iostream>
#include <vector>

void counting_sort(std::vector<int> &a, std::vector<int> &temp) {
    int n = a.size();
    int mx = 10;
    std::vector<int> c(mx);
    for (int i = 0; i < n; ++i) {
        ++c[temp[i] % 10];
    }
    for (int i = 1; i < mx; ++i) {
        c[i] += c[i - 1];
    }
    std::vector<int> ans(n);
    std::vector<int> ans_temp(n);
    for (int i = n - 1; i >= 0; --i) {
        ans[c[temp[i] % 10] - 1] = a[i];
        ans_temp[--c[temp[i] % 10]] = temp[i];
    }
    a = ans;
    temp = ans_temp;
}

int main() {
    #ifdef LOCAL
        freopen("input.txt", "r", stdin);
    #endif
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    for (auto &x : a) {
        std::cin >> x;
    }
    std::vector<int> temp = a;
    
    for (int i = 0; i < 10; ++i) {
        if (i > 0) {
            for (auto &x : temp) {
                x /= 10;
            }
        }
        counting_sort(a, temp);
    }
    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}

// здесь есть доп массив из-за того, что без битовой оптимизации нельзя брать k-й разряд быстро через модульную арифметику
