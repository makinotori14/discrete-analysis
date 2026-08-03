class BST {
public:
    struct Node {
        int val;
        Node* par;
        Node* left;
        Node* right;

        Node(int key, Node* parent = nullptr) : val(key), par(parent), left(nullptr), right(nullptr) {}
    };

private:
    Node* root = nullptr;

    Node* Min(Node* v) {
        Node* cur = v;
        while (cur != nullptr && cur->left != nullptr) {
            cur = cur->left;
        }
        return cur;
    }

    void Transplant(Node* old_node, Node* new_node) {
        Node* p = old_node->par;
        if (p == nullptr) {
            root = new_node;
        } else if (p->left == old_node) {
            p->left = new_node;
        } else {
            p->right = new_node;
        }

        if (new_node != nullptr) {
            new_node->par = p;
        }
    }

    void Destroy(Node* v) {
        if (v == nullptr) {
            return;
        }
        Destroy(v->left);
        Destroy(v->right);
        delete v;
    }

    Node* FindNode(int v) {
        Node* cur = root;
        while (cur != nullptr) {
            if (v < cur->val) {
                cur = cur->left;
            } else if (v > cur->val) {
                cur = cur->right;
            } else {
                return cur;
            }
        }
        return cur;
    }

    void RemoveNode(Node* v) {
        if (v == nullptr) {
            return;
        }

        if (v->left == nullptr) {
            Transplant(v, v->right);
        } else if (v->right == nullptr) {
            Transplant(v, v->left);
        } else {
            Node* m = Min(v->right);
            v->val = m->val;
            RemoveNode(m);
            return;
        }

        delete v;
    }

    Node* Successor(Node* v) {
        if (v->right != nullptr) {
            return Min(v->right);
        }
        Node* cur = v;
        while (cur->par != nullptr) {
            if (cur->par->left == cur) {
                return cur->par;
            } else {
                cur = cur->par;
            }
        }
        return nullptr;
    }

public:
    BST() = default;

    ~BST() {
        Destroy(root);
    }
    
    // No copy constructor
    BST(const BST&) = delete;
    
    // No copy operator
    BST& operator=(const BST&) = delete;
    
    bool Insert(int val) {
        Node* cur = root;
        Node* par = nullptr;
        while (cur != nullptr) {
            par = cur;
            if (cur->val == val) {
                return false;
            } else if (val < cur->val) {
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        Node* new_node = new Node(val, par);

        if (par == nullptr) {
            root = new_node;
        } else if (val < par->val) {
            par->left = new_node;
        } else {
            par->right = new_node;
        }
        return true;
    }

    bool Remove(int x) {
        Node* v = FindNode(x);
        if (v == nullptr) {
            return false;
        }
        RemoveNode(v);
        return true;
    }

    bool Find(int x) {
        Node* v = FindNode(x);
        return v != nullptr;
    }
};
