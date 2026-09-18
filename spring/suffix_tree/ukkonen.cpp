#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

class SuffixTree {
public:
    struct Node {
        int* l = nullptr;
        int* r = nullptr;
        Node* sufLink = nullptr;
        Node* par = nullptr;
        const int ALP = 27;
        std::vector<Node*> c;

        Node(Node* sufLink, Node* par) : sufLink(sufLink), par(par) {
            c.assign(ALP, nullptr);
        }
    };

    int lv(Node* x) {
        return *(x->l);
    }

    int rv(Node* x) {
        return *(x->r);
    }

private:
    Node* root = new Node(nullptr, nullptr);

    int* end = new int(0);

    Node* activeNode = root;
    int activeEdge = 0;
    int activeLen = 0;

    Node* lastExplicitNode = nullptr;

    int getId(char x) {
        return x == '$' ? 0 : x - 'a' + 1;
    }

    void skipCountTrick(std::string &s, int backLeft, int backRight) {
        int cur = backLeft;
        int rem = backRight - backLeft + 1;
        while (rem > 0) {
            char x = s[cur];
            Node* next = activeNode->c[getId(x)];
            int sz = rv(next) - lv(next) + 1;
            if (sz < rem) {
                rem -= sz;
                cur += sz;
                activeNode = next;
            } else if (sz == rem) {
                activeNode = next;
                activeLen = 0;
                return;
            } else {
                activeLen = rem;
                activeEdge = getId(x);
                return;
            }
        }
    }

    int TreeExtend(std::string &s, int id) {
        *end = id;
        char x = s[id];
        
        int backLeft = 0;
        int backRight = 0;

        if (activeLen == 0) {
            if (activeNode->c[getId(x)] != nullptr) {
                if (lastExplicitNode != nullptr) {
                    lastExplicitNode->sufLink = activeNode;
                    lastExplicitNode = nullptr;
                }
                Node* next = activeNode->c[getId(x)];
                if (rv(next) - lv(next) == 0) {
                    activeNode = next;
                } else {
                    ++activeLen;
                    activeEdge = getId(x);
                }
                return 3;
            } else {
                if (lastExplicitNode != nullptr) {
                    lastExplicitNode->sufLink = activeNode;
                    lastExplicitNode = nullptr;
                }
                activeNode->c[getId(x)] = new Node(root, activeNode);
                Node* newNode = activeNode->c[getId(x)];
                newNode->l = new int(id);
                newNode->r = end;

                backLeft = lv(activeNode);
                backRight = rv(activeNode);

                if (activeNode == root) {
                    return 0;
                }

                if (activeNode != root && activeNode->par == root) {
                    ++backLeft;
                    if (backRight - backLeft + 1 == 0) {
                        activeNode = root;
                        return 2;
                    }
                }

                activeNode = activeNode->par->sufLink;
                skipCountTrick(s, backLeft, backRight);
            }
        } else {
            Node* next = activeNode->c[activeEdge];
            int check = lv(next) + activeLen;
            if (s[check] == x) {
                ++activeLen;
                if (rv(next) - lv(next) + 1 == activeLen) {
                    activeLen = 0;
                    activeNode = next;
                }
                return 3;
            } else {
                Node* split = new Node(root, activeNode);
                if (lastExplicitNode != nullptr) {
                    lastExplicitNode->sufLink = split;
                    lastExplicitNode = nullptr;
                }
                lastExplicitNode = split;
                split->l = new int(lv(next));
                split->r = new int(check - 1);
                split->par = activeNode;
                activeNode->c[activeEdge] = split;

                split->c[getId(s[check])] = next;
                next->par = split;
                *(next->l) = check;

                split->c[getId(x)] = new Node(root, split);
                Node* newNode = split->c[getId(x)];
                newNode->l = new int(id);
                newNode->r = end;

                backLeft = lv(split);
                backRight = rv(split);

                activeNode = split->par->sufLink;
                if (split->par == root) {
                    ++backLeft;
                    if (backRight - backLeft + 1 == 0) {
                        activeLen = 0;
                        return 2;
                    }
                }
                skipCountTrick(s, backLeft, backRight);
            }
        }

        return 2;
    }

    void Build(std::string &s) {
        for (int i = 0; i < s.size(); ++i) {
            lastExplicitNode = nullptr;
            while (TreeExtend(s, i) == 2) {}
        }
    }

    void Clear(Node* cur) {
        if (cur == nullptr) {
            return;
        }
        for (int i = 0; i < 27; ++i) {
            Clear(cur->c[i]);
        }
        delete cur->l;
        if (cur->r != end) {
            delete cur->r;
        }
        delete cur;
    }

    void dfs(Node* cur, int d) {
        if (cur != root) {
            std::cout << d << ' ' << lv(cur) + 1 << ' ' << rv(cur) + 1 << '\n';
        }
        for (int i = 0; i < 27; ++i) {
            if (cur->c[i] == nullptr) {
                continue;
            }
            dfs(cur->c[i], d + 1);
        }
    }

public:
    SuffixTree(std::string s) {
        if (s.empty()) {
            throw std::runtime_error("no empty strings :D");
        }
        root->l = new int(-1);
        root->r = new int(-1);

        root->par = root;
        root->sufLink = root;
        // string doesn't contain dollar symbol
        s += '$';
        Build(s);
    }

    ~SuffixTree() {
        Clear(root);
        delete end;
    }

    void getAns() {
        dfs(root, -1);
    }

    SuffixTree(const SuffixTree&) = delete;
    SuffixTree operator=(const SuffixTree&) = delete;
};

void solve() {
    std::string s;
    std::cin >> s;

    SuffixTree t(s);
    t.getAns();
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        solve();
    }
}