#include <random>
#include <chrono>
#include <cstdint>
#include <utility>

class Treap {
public:
    struct Node {
        int key;
        uint32_t p;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* par;

        Node(int key, uint32_t p, Node* par = nullptr) : key(key), p(p), par(par) {};
    };
private:
    Node* root = nullptr;
    std::mt19937 gen;
    double eps = 1e-3;

    Node* FindNode(int key) {
        Node* cur = root;
        while (cur != nullptr) {
            if (key < cur->key) {
                cur = cur->left;
            } else if (key > cur->key) {
                cur = cur->right;
            } else {
                return cur;
            }
        }
        return cur;
    }

    std::pair<Node*, Node*> SplitEngine(Node* T, double key) {
        if (T == nullptr) {
            return {nullptr, nullptr};
        }
        if (key > T->key) {
            // split right
            auto [R1, R2] = SplitEngine(T->right, key);
            T->right = R1;
            if (R1 != nullptr) {
                R1->par = T;
            }
            if (R2 != nullptr) {
                R2->par = nullptr;
            }
            return {T, R2};
        } else {
            //split left
            auto [L1, L2] = SplitEngine(T->left, key);
            T->left = L2;
            if (L2 != nullptr) {
                L2->par = T;
            }
            if (L1 != nullptr) {
                L1->par = nullptr;
            }
            return {L1, T};
        }
    }

    std::pair<Node*, Node*> Split(Node* T, double key) {
        auto [T1, T2] = SplitEngine(T, key);
        if (T1 != nullptr) { 
            T1->par = nullptr;
        }
        if (T2 != nullptr) {
            T2->par = nullptr;
        }

        return {T1, T2};
    }

    Node* MergeEngine(Node* T1, Node* T2) {
        if (T1 == nullptr) {
            return T2;
        } else if (T2 == nullptr) {
            return T1;
        }
        if (T1->p > T2->p) {
            Node* m = MergeEngine(T1->right, T2);
            if (T1 != nullptr) {
                T1->right = m;
            }
            if (m != nullptr) {
                m->par = T1;
            }
            return T1;
        } else {
            Node* m = MergeEngine(T1, T2->left);
            if (T2 != nullptr) {
                T2->left = m;
            }
            if (m != nullptr) {
                m->par = T2;
            }
            return T2;
        }
    }

    Node* Merge(Node* T1, Node* T2) {
        Node* T = MergeEngine(T1, T2);
        if (T != nullptr) {
            T->par = nullptr;
        }
        return T;
    }

    void Clear(Node* v) {
        if (v == nullptr) {
            return;
        }
        Clear(v->left);
        Clear(v->right);
        delete v;
    }

public:
    Treap(uint_fast32_t seed=std::chrono::steady_clock::now().time_since_epoch().count()) : gen(seed) {}

    ~Treap() {
        Clear(root);
    }

    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;

    bool Find(int key) {
        return FindNode(key) != nullptr;
    }

    bool Insert(int key, int type=1) {
        if (Find(key)) {
            return false;
        }
        uint32_t p = gen();
        if (type == 1) {
            Node* K = new Node(key, p);
            auto [T1, T2] = Split(root, key);
            Node* T1p = Merge(T1, K);
            Node* T = Merge(T1p, T2);
            root = T;
        } else {
            if (root == nullptr) {
                root = new Node(key, p);
                return true;
            }

            Node* cur = root;
            Node* par = nullptr;
            char dir = 0;
            while (cur != nullptr && cur->p > p) {
                par = cur;
                if (key < cur->key) {
                    cur = cur->left;
                    dir = 0;
                } else {
                    cur = cur->right;
                    dir = 1;
                }
            }
            Node* K = new Node(key, p);
            auto [T1, T2] = Split(cur, key);
            K->left = T1;
            K->right = T2;

            if (T1 != nullptr) {
                T1->par = K;
            }
            if (T2 != nullptr) {
                T2->par = K;
            }

            K->par = par;
            if (par == nullptr) {
                root = K;
            }
            if (par != nullptr) {
                if (dir == 0) {
                    par->left = K;
                } else {
                    par->right = K;
                }
            }
        }

        return true;
    }

    bool Delete(int key, int type=1) {
        if (!Find(key)) {
            return false;
        }

        if (type == 1) {
            auto [T1, T2] = Split(root, key);
            auto [R1, R2] = Split(T2, key + eps);
            Node* T = Merge(T1, R2);
            root = T;

            Clear(R1);
        } else {
            Node* T = FindNode(key);
            Node* par = T->par;

            char dir = 0;
            if (par != nullptr && par->right == T) {
                dir = 1;
            }

            Node* T_new = Merge(T->left, T->right);
            if (T_new != nullptr) {
                T_new->par = par;
            }
            if (par == nullptr) {
                root = T_new;
            } else {
                if (dir == 0) {
                    par->left = T_new;
                } else {
                    par->right = T_new;
                }
            }
            
            delete T;
        }

        return true;
    }
};