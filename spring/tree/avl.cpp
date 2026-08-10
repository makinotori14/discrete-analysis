class AVL {
public:
    struct Node {
        int key = 0;
        int bf = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* par = nullptr;
        
        Node(int x, Node* p) {
            key = x;
            par = p;
        }
    };
private:
    Node* root = nullptr;
    
    Node* FindNode(int x) {
        Node* cur = root;
        while (cur != nullptr) {
            if (x < cur->key) {
                cur = cur->left;
            } else if (x > cur->key) {
                cur = cur->right;
            } else {
                break;
            }
        }
        return cur;
    }

    Node* FindMin(Node *v) {
        Node* cur = v;
        while (cur != nullptr && cur->left != nullptr) {
            cur = cur->left;
        }
        return cur;
    }

    void BalanceDelete(Node* v, int d) {
        Node* cur = v;
        cur->bf += d;
        while (cur != nullptr && cur->bf != -1 && cur->bf != 1) {
            if (cur->bf == 0) {
                if (cur->par == nullptr) {
                    break;
                } else if (cur->par->left == cur) {
                    cur = cur->par;
                    cur->bf--;
                } else if (cur->par->right == cur) {
                    cur = cur->par;
                    cur->bf++;
                }
            } else if (cur->bf == 2) {
                if (cur->left->bf == 1) {
                    Node* x = cur;
                    Node* y = cur->left;
                    RightRotate(x, y);
                    x->bf = 0;
                    y->bf = 0;
                    cur = y->par;
                    if (cur == nullptr) {
                        break;
                    }
                    if (cur->left == y) {
                        cur->bf--;
                    } else {
                        cur->bf++;
                    }
                } else if (cur->left->bf == -1) {
                    Node* x = cur;
                    Node* y = cur->left;
                    Node* z = cur->left->right;
                    int s = z->bf;
                    LeftRotate(y, z);
                    RightRotate(x, z);
                    z->bf = 0;
                    if (s == -1) {
                        y->bf = 1;
                        x->bf = 0;
                    } else if (s == 0) {
                        y->bf = 0;
                        x->bf = 0;
                    } else if (s == 1) {
                        y->bf = 0;
                        x->bf = -1;
                    }
                    cur = z->par;
                    if (cur == nullptr) {
                        break;
                    }
                    if (cur->left == z) {
                        cur->bf--;
                    } else {
                        cur->bf++;
                    }
                } else if (cur->left->bf == 0) {
                    Node* x = cur;
                    Node* y = cur->left;
                    RightRotate(x, y);
                    x->bf = 1;
                    y->bf = -1;
                    break;
                }
            } else if (cur->bf == -2) {
                if (cur->right->bf == -1) {
                    Node* x = cur;
                    Node* y = cur->right;
                    LeftRotate(x, y);
                    x->bf = 0;
                    y->bf = 0;
                    cur = y->par;
                    if (cur == nullptr) {
                        break;
                    }
                    if (cur->left == y) {
                        cur->bf--;
                    } else {
                        cur->bf++;
                    }
                } else if (cur->right->bf == 1) {
                    Node* x = cur;
                    Node* y = cur->right;
                    Node* z = cur->right->left;
                    int s = z->bf;
                    RightRotate(y, z);
                    LeftRotate(x, z);
                    z->bf = 0;
                    if (s == -1) {
                        y->bf = 0;
                        x->bf = 1;
                    } else if (s == 0) {
                        y->bf = 0;
                        x->bf = 0;
                    } else if (s == 1) {
                        y->bf = -1;
                        x->bf = 0;
                    }
                    cur = z->par;
                    if (cur == nullptr) {
                        break;
                    }
                    if (cur->left == z) {
                        cur->bf--;
                    } else {
                        cur->bf++;
                    }
                } else if (cur->right->bf == 0) {
                    Node* x = cur;
                    Node* y = cur->right;
                    LeftRotate(x, y);
                    x->bf = -1;
                    y->bf = 1;
                    break;
                }
            }
        } 
    }

    void DeleteNode(Node* v) {
        if (v->left == nullptr && v->right == nullptr) {
            if (v->par == nullptr) {
                root = nullptr;
            } else {
                if (v->par->right == v) {
                    v->par->right = nullptr;
                    BalanceDelete(v->par, 1);
                } else {
                    v->par->left = nullptr;
                    BalanceDelete(v->par, -1);
                }
            }
            delete v;
            return;
        }
        if (v->left == nullptr && v->right != nullptr) {
            if (v->par == nullptr) {
                root = v->right;
                v->right->par = nullptr;
            } else {
                if (v->par->right == v) {
                    v->par->right = v->right;
                    v->right->par = v->par;
                    BalanceDelete(v->par, 1);
                } else {
                    v->par->left = v->right;
                    v->right->par = v->par;
                    BalanceDelete(v->par, -1);
                }
            }
            delete v;
            return;
        }
        if (v->right == nullptr && v->left != nullptr) {
            if (v->par == nullptr) {
                root = v->left;
                v->left->par = nullptr;
            } else {
                if (v->par->right == v) {
                    v->par->right = v->left;
                    v->left->par = v->par;
                    BalanceDelete(v->par, 1);
                } else {
                    v->par->left = v->left;
                    v->left->par = v->par;
                    BalanceDelete(v->par, -1);
                }
            }
            delete v;
            return;
        }
        
        Node* m = FindMin(v->right);
        v->key = m->key;
        DeleteNode(m);
    }

    void LeftRotate(Node* x, Node* y) {
        Node* temp = y->left;
        y->left = x;
        y->par = x->par;
        if (y->par != nullptr) {
            if (y->par->left == x) {
                y->par->left = y;
            } else {
                y->par->right = y;
            }
        } else {
            root = y;
        }
        x->par = y;
        x->right = temp;
        if (x->right != nullptr) {
            x->right->par = x;
        }
    }

    void RightRotate(Node* x, Node* y) {
        Node* temp = y->right;
        y->right = x;
        y->par = x->par;
        if (y->par != nullptr) {
            if (y->par->left == x) {
                y->par->left = y;
            } else {
                y->par->right = y;
            }
        } else {
            root = y;
        }
        x->par = y;
        x->left = temp;
        if (x->left != nullptr) {
            x->left->par = x;
        }
    }

    void BalanceInsert(Node* v, int d) {
        Node* cur = v;
        cur->bf += d;
        while (cur != nullptr && cur->bf != 0) {
            if (cur->bf == 1 || cur->bf == -1) {
                if (cur->par == nullptr) {
                    break;
                } else if (cur->par->left == cur) {
                    cur = cur->par;
                    cur->bf++;
                } else if (cur->par->right == cur) {
                    cur = cur->par;
                    cur->bf--;
                }
            } else if (cur->bf == 2) {
                if (cur->left->bf == 1) {
                    Node* x = cur;
                    Node* y = cur->left;
                    RightRotate(x, y);
                    x->bf = 0;
                    y->bf = 0;
                } else if (cur->left->bf == -1) {
                    Node* x = cur;
                    Node* y = cur->left;
                    Node* z = cur->left->right;
                    int s = z->bf;
                    LeftRotate(y, z);
                    RightRotate(x, z);
                    z->bf = 0;
                    if (s == -1) {
                        y->bf = 1;
                        x->bf = 0;
                    } else if (s == 0) {
                        y->bf = 0;
                        x->bf = 0;
                    } else if (s == 1) {
                        y->bf = 0;
                        x->bf = -1;
                    }
                } else if (cur->left->bf == 0) {
                    Node* x = cur;
                    Node* y = cur->left;
                    RightRotate(x, y);
                    x->bf = 1;
                    y->bf = -1;
                }
                break;
            } else if (cur->bf == -2) {
                if (cur->right->bf == -1) {
                    Node* x = cur;
                    Node* y = cur->right;
                    LeftRotate(x, y);
                    x->bf = 0;
                    y->bf = 0;
                } else if (cur->right->bf == 1) {
                    Node* x = cur;
                    Node* y = cur->right;
                    Node* z = cur->right->left;
                    int s = z->bf;
                    RightRotate(y, z);
                    LeftRotate(x, z);
                    z->bf = 0;
                    if (s == -1) {
                        y->bf = 0;
                        x->bf = 1;
                    } else if (s == 0) {
                        y->bf = 0;
                        x->bf = 0;
                    } else if (s == 1) {
                        y->bf = -1;
                        x->bf = 0;
                    }
                } else if (cur->right->bf == 0) {
                    Node* x = cur;
                    Node* y = cur->right;
                    LeftRotate(x, y);
                    x->bf = -1;
                    y->bf = 1;
                }
                break;
            }
        } 
    }

public:
    bool Find(int x) {
        return FindNode(x) != nullptr;
    }

    bool Insert(int x) {
        Node* cur = root;
        Node* p = nullptr;
        while (cur != nullptr) {
            p = cur;
            if (x < cur->key) {
                cur = cur->left;
            } else if (x > cur->key) {
                cur = cur->right;
            } else {
                return false;
            }
        }
        if (p == nullptr) {
            root = new Node(x, nullptr);
            return true;
        }
        if (x < p->key) {
            p->left = new Node(x, p);
            BalanceInsert(p, 1);
        } else {
            p->right = new Node(x, p);
            BalanceInsert(p, -1);
        }

        return true;
    } 

    bool Delete(int x) {
        Node* d = FindNode(x);
        if (d == nullptr) {
            return false;
        }
        DeleteNode(d);
        return true;
    }
};
