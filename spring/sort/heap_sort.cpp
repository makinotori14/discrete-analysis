#include <iostream>
#include <vector>
#include <algorithm>

struct BinaryHeap {
    std::vector<int> h;
    
    void siftDown(int i) {
        while ((2 * i + 1 < h.size() && h[2 * i + 1] > h[i]) ||
        (2 * i + 2 < h.size() && h[2 * i + 2] > h[i])) {
            int next = i;
            if (2 * i + 1 < h.size() && h[2 * i + 1] > h[next]) {
                next = 2 * i + 1;
            }
            if (2 * i + 2 < h.size() && h[2 * i + 2] > h[next]) {
                next = 2 * i + 2;
            }
            std::swap(h[i], h[next]);
            i = next;
        }
    }

    int getMax() {
        int ans = h[0];
        h[0] = h[h.size() - 1];
        h.pop_back();
        siftDown(0);
        return ans;
    }

    void Build(std::vector<int> a) {
        for (auto &x : a) {
            h.push_back(x);
        }
        for (int i = h.size() - 1; i >= 0; --i) {
            siftDown(i);
        }
    }
};

void heap_sort(std::vector<int> &a) {
    BinaryHeap heap;
    heap.Build(a);
    int n = a.size();
    for (int i = 0; i < n; ++i) {
        a[n - i - 1] = heap.getMax();
    }
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);

    for (auto &x : a) {
        std::cin >> x;
    }
    heap_sort(a);
    for (auto &x : a) {
        std::cout << x << ' ';
    }
    std::cout << '\n';
}