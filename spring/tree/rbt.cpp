class RBT {
public:
    struct Node {
        enum class Color {
            Red,
            Black
        };
        
        int key;
        Node* par = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
        Color color = Color::Red;

        Node(int key, Node* par=nullptr, Color color=Color::Red) : key(key), par(par), color(color) {}

        void ToBlack() {
            color = Color::Black;
        }

        void ToRed() {
            color = Color::Red;
        }
    };

private:
    Node* root = nullptr;

    void Clear(Node* v) {
        if (v == nullptr) {
            return;
        }
        Clear(v->left);
        Clear(v->right);
        delete v;
    }

    Node* P(Node* v) {
        return v->par;
    }

    Node* GP(Node* v) {
        return P(v)->par;
    }

    Node* U(Node* v) {
        Node* gp = GP(v);
        Node* p = P(v);
        if (gp->left == p) {
            return gp->right;
        } else {
            return gp->left;
        }
    }

    Node* B(Node* v, Node* par = nullptr) {
        Node* p;
        if (v != nullptr) {
            p = P(v);
        } else {
            p = par;
        }
        if (p->left == v) {
            return p->right;
        } else {
            return p->left;
        }
    }

    Node* FarBSon(Node* v, Node* par = nullptr) {
        Node* b = B(v, par);
        if (par->left == v) {
            return b->right;
        } else {
            return b->left;
        }
    }

    Node* NearBSon(Node* v, Node* par = nullptr) {
        Node* b = B(v, par);
        if (par->left == v) {
            return b->left;
        } else {
            return b->right;
        }
    }

    bool IsBlack(Node* v) {
        return v == nullptr || v->color == Node::Color::Black;
    }

    bool IsRed(Node* v) {
        return v != nullptr && v->color == Node::Color::Red;
    }

    void SwitchColor(Node* x, Node* y) {
        Node::Color temp = x->color;
        x->color = y->color;
        y->color = temp;
    }

    void LeftRotate(Node* x, Node* y) {
        y->par = x->par;
        if (y->par == nullptr) {
            root = y;
        } else {
            if (y->par->left == x) {
                y->par->left = y;
            } else {
                y->par->right = y;
            }
        }
        x->right = y->left;
        y->left = x;
        
        if (x->right != nullptr) {
            x->right->par = x;
        }
        x->par = y;
    }

    void RightRotate(Node* x, Node* y) {
        y->par = x->par;
        if (y->par == nullptr) {
            root = y;
        } else {
            if (y->par->left == x) {
                y->par->left = y;
            } else {
                y->par->right = y;
            }
        }

        x->left = y->right;
        y->right = x;

        if (x->left != nullptr) {
            x->left->par = x;
        }
        x->par = y;
    }

    void Rotate(Node* old_node, Node* new_node) {
        if (old_node->left == new_node) {
            RightRotate(old_node, new_node);
        } else {
            LeftRotate(old_node, new_node);
        }
    }

    Node* FindNode(int x) {
        Node* cur = root;
        while (cur != nullptr) {
            if (x < cur->key) {
                cur = cur->left;
            } else if (x > cur->key) {
                cur = cur->right;
            } else {
                return cur;
            }
        }
        return cur;
    }

    void BalanceInsert(Node* v) {
        if (v == root) {
            v->ToBlack();
            return;
        }
        if (IsBlack(P(v))) {
            return;
        }

        if (IsRed(U(v))) {
            U(v)->ToBlack();
            P(v)->ToBlack();
            GP(v)->ToRed();

            BalanceInsert(GP(v));
        } else {
            if (GP(v)->left == P(v) && P(v)->right == v || GP(v)->right == P(v) && P(v)->left == v) {
                Node* next = P(v);
                Rotate(P(v), v);
                v = next;
            }

            P(v)->ToBlack();
            GP(v)->ToRed();
            Rotate(GP(v), P(v));
        }
    }

    // run only if Deleted Node is BLACK
    void BalanceDelete(Node* v, Node* p) {
        if (IsRed(v)) {
            v->ToBlack();
            return;
        }

        // Make Black Brother (it always exists)
        if (IsRed(B(v, p))) {
            p->ToRed();
            B(v, p)->ToBlack();
            Rotate(p, B(v, p));
        }

        if (IsBlack(NearBSon(v, p)) && IsBlack(FarBSon(v, p))) {
            B(v, p)->ToRed();
            if (p == root || IsRed(p)) {
                p->ToBlack();
                return;
            } else {
                BalanceDelete(p, P(p));
                return;
            }
        }
        
        if (IsRed(NearBSon(v, p)) && IsBlack(FarBSon(v, p))) {
            B(v, p)->ToRed();
            NearBSon(v, p)->ToBlack();
            Rotate(B(v, p), NearBSon(v, p));
        }

        if (IsRed(FarBSon(v, p))) {
            SwitchColor(p, B(v, p));
            FarBSon(v, p)->ToBlack();
            Rotate(p, B(v, p));
        }
    }

    Node* FindMin(Node* v) {
        Node* cur = v;
        while (cur != nullptr && cur->left != nullptr) {
            cur = cur->left;
        }
        return cur;
    }

    void DeleteNode(Node* v) {
        if (v->left == nullptr && v->right == nullptr) {
            if (P(v) == nullptr) {
                root = nullptr;
            } else {
                if (P(v)->left == v) {
                    P(v)->left = nullptr;
                } else {
                    P(v)->right = nullptr;
                }
                if (IsBlack(v)) {
                    BalanceDelete(nullptr, P(v));
                }
            }
            delete v;
            return;
        }

        if (v->left != nullptr && v->right == nullptr) {
            Node* next = v->left;
            if (P(v) == nullptr) {
                root = next;
            } else {
                if (P(v)->left == v) {
                    P(v)->left = next;
                } else {
                    P(v)->right = next;
                }
            }
            next->par = P(v);
            if (IsBlack(v)) {
                BalanceDelete(next, P(next));
            }
            delete v;
            return;  
        }

        if (v->left == nullptr && v->right != nullptr) {
            Node* next = v->right;
            if (P(v) == nullptr) {
                root = next;
            } else {
                if (P(v)->left == v) {
                    P(v)->left = next;
                } else {
                    P(v)->right = next;
                }
            }
            next->par = P(v);
            if (IsBlack(v)) {
                BalanceDelete(next, P(next));
            }
            delete v;
            return;  
        }

        Node* m = FindMin(v->right);
        v->key = m->key;
        DeleteNode(m);
    }
public:
    RBT() = default;

    ~RBT() {
        Clear(root);
    }

    RBT(const RBT&) = delete;
    RBT& operator=(const RBT&) = delete;

    bool Find(int x) {
        return FindNode(x) != nullptr;
    }

    bool Insert(int x) {
        Node* par = nullptr;
        Node* cur = root;
        while (cur != nullptr) {
            par = cur;
            if (x < cur->key) {
                cur = cur->left;
            } else if (x > cur->key) {
                cur = cur->right;
            } else {
                return false;
            }
        }
        if (par == nullptr) {
            root = new Node(x);
            root->ToBlack();
        } else {
            cur = new Node(x, par);
            if (x < par->key) {
                par->left = cur;
            } else {
                par->right = cur;
            }
            BalanceInsert(cur);
        }
        return true;
    }

    bool Delete(int x) {
        Node* v = FindNode(x);
        if (v == nullptr) {
            return false;
        }
        DeleteNode(v);
        return true;
    }
};
