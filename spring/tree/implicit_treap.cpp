#include <utility>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <cstdint>

class ImpTreap {
public:
    struct Node {
        int val;
        int sz = 1;
        uint32_t p;
        Node* left = nullptr;
        Node* right = nullptr;

        // Extra
        bool rev = 0;
        int added = 0;

        // Queries
        int sum;
        int mx;

        Node(int val, uint32_t p) : val(val), p(p), sum(val), mx(val) {}
    };

private:
    Node* root = nullptr;
    std::mt19937 gen{1337};
    const int INF = 1e9 + 7;

    int Size(Node* T) {
        return (T == nullptr ? 0 : T->sz);
    }

    int Sum(Node* T) {
        return (T == nullptr ? 0 : T->sum);
    }

    int Max(Node* T) {
        return (T == nullptr ? -INF: T->mx);
    }

    void Update(Node* T) {
        T->sz = Size(T->left) + Size(T->right) + 1;
        T->sum = Sum(T->left) + Sum(T->right) + T->val;
        T->mx = std::max(std::max(Max(T->left), Max(T->right)), T->val);
    }

    void Push(Node* T) {
        if (T == nullptr) {
            return;
        }

        if (T->rev == 1) {
            ApplyReverse(T->left);
            ApplyReverse(T->right);
            T->rev = 0;
        }

        if (T->added != 0) {
            ApplyAdd(T->left, T->added);
            ApplyAdd(T->right, T->added);
            T->added = 0;
        }
    }
    
    std::pair<Node*, Node*> Split(Node* T, int k) {
        if (T == nullptr) {
            return {nullptr, nullptr};
        }

        Push(T);

        int lsz = Size(T->left);
        if (k <= lsz) {
            auto [L1, L2] = Split(T->left, k);
            T->left = L2;
            Update(T);
            return {L1, T};
        } else {
            auto [R1, R2] = Split(T->right, k - lsz - 1);
            T->right = R1;
            Update(T);
            return {T, R2};
        }
    }

    Node* Merge(Node* T1, Node* T2) {
        if (T1 == nullptr) {
            return T2;
        } else if (T2 == nullptr) {
            return T1;
        }

        if (T1->p > T2->p) {
            Push(T1);

            T1->right = Merge(T1->right, T2);
            Update(T1);
            return T1;
        } else {
            Push(T2);

            T2->left = Merge(T1, T2->left);
            Update(T2);
            return T2;
        }
    }

    void ApplyReverse(Node* T) {
        if (T == nullptr) {
            return;
        }
        T->rev ^= 1;
        std::swap(T->left, T->right);
    }

    void ApplyAdd(Node* T, int x) {
        if (T == nullptr) {
            return;
        }
        T->val += x;
        T->added += x;
        T->sum += x * Size(T);
        T->mx += x;
    }

    void Clear(Node* T) {
        if (T == nullptr) {
            return;
        }
        Clear(T->left);
        Clear(T->right);
        delete T;
    }

public:
    ImpTreap() = default;

    ImpTreap(const ImpTreap&) = delete;
    ImpTreap& operator=(const ImpTreap&) = delete;

    ~ImpTreap() {
        Clear(root);
    }

    // find[i], insert, delete, reverse, sum
    int Find(int i) {
        // or just go down the tree

        if (i < 0 || i >= Size(root)) {
            throw std::runtime_error("Out of range");
        }
        auto [T1, T2] = Split(root, i);
        auto [L, R] = Split(T2, 1);
        int ans = L->val;
        T2 = Merge(L, R);
        root = Merge(T1, T2);
        return ans;
    }

    void Insert(int x, int i) {
        if (i < 0 || i > Size(root)) {
            throw std::runtime_error("Out of range");
        }
        auto [T1, T2] = Split(root, i);
        Node* K = new Node(x, gen());
        T1 = Merge(T1, K);
        root = Merge(T1, T2);
    }

    void Delete(int i) {
        if (i < 0 || i >= Size(root)) {
            throw std::runtime_error("Out of range");
        }
        auto [T1, T2] = Split(root, i);
        auto [L, R] = Split(T2, 1);

        root = Merge(T1, R);

        delete L;
    }

    void Reverse(int l, int r) {
        // [l, r);
        if (r < l || l < 0 || r > Size(root)) {
            throw std::runtime_error("Shit range");
        }

        auto [T1, rest] = Split(root, l);
        auto [T2, T3] = Split(rest, r - l);

        ApplyReverse(T2);

        root = Merge(Merge(T1, T2), T3);
    }

    void Add(int l, int r, int x) {
        // [l, r);
        if (r < l || l < 0 || r > Size(root)) {
            throw std::runtime_error("Shit range");
        }

        auto [T1, rest] = Split(root, l);
        auto [T2, T3] = Split(rest, r - l);

        ApplyAdd(T2, x);

        root = Merge(Merge(T1, T2), T3);
    }

    int GetSum(int l, int r) {
        if (r < l || l < 0 || r > Size(root)) {
            throw std::runtime_error("Shit range");
        }

        auto [T1, rest] = Split(root, l);
        auto [T2, T3] = Split(rest, r - l);

        int ans = Sum(T2);

        root = Merge(Merge(T1, T2), T3);

        return ans;
    }

    int GetMax(int l, int r) {
        if (r <= l || l < 0 || r > Size(root)) {
            throw std::runtime_error("Shit range");
        }

        auto [T1, rest] = Split(root, l);
        auto [T2, T3] = Split(rest, r - l);

        int ans = Max(T2);

        root = Merge(Merge(T1, T2), T3);

        return ans;
    }
};