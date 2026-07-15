#include <algorithm>
#include <stdexcept>

const int SIZE = 1e7;

// лучше сделать на vector, массив тут был для упражнения. массив такого размера может переполнить стек

struct BinaryHeap {
    int heap[SIZE];
    int sz = 0;

    void siftDown(int i) {
        while ((2 * i + 1 < sz && heap[i] < heap[2 * i + 1]) || 
        (2 * i + 2 < sz && heap[i] < heap[2 * i + 2])) {
            int next = i;
            if (2 * i + 1 < sz && heap[2 * i + 1] > heap[next]) {
                next = 2 * i + 1;
            }
            if (2 * i + 2 < sz && heap[2 * i + 2] > heap[next]) {
                next = 2 * i + 2;
            }
            std::swap(heap[i], heap[next]);
            i = next;
        }
    }

    void siftUp(int i) {
        while (i != 0) {
            int next = (i - 1) / 2;
            if (heap[next] < heap[i]) {
                std::swap(heap[next], heap[i]);
                i = next;
            } else {
                break;
            }
        }
    }

    int getMax() {
        if (sz == 0) {
            throw std::runtime_error("it's empty");
        }
        int ans = heap[0];
        heap[0] = heap[sz - 1];
        --sz;
        siftDown(0);
        return ans;
    }

    void Insert(int x) {
        if (sz == SIZE) {
            throw std::runtime_error("it's full");
        }
        heap[sz++] = x;
        siftUp(sz - 1);
    }

    void Build(const int *a, int n) {
        sz = 0;
        for (int i = 0; i < n; ++i) {
            heap[i] = a[i];
            ++sz;
        }
        for (int i = sz / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
    } 
};