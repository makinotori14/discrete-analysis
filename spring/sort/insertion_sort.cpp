#include <iostream>
#include <vector>

void insertion_sort(std::vector<int> &a) {
    for (int i = 1; i < (int)a.size(); ++i) {
        int key = a[i];
        for (int j = i - 1; j >= -1; --j) {
            if (j == -1 || a[j] <= key) {
                a[j + 1] = key;
                break;
            } else {
                a[j + 1] = a[j];
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
    insertion_sort(a);

    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}