#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <stdexcept>
#include <map>

class AhoKor {
public:
    struct Node {
        const int ALP = 26;
        int term = -1;
        std::vector<Node*> v;
        Node* f = nullptr;
        Node* e = nullptr;

        Node() {
            v.assign(ALP, nullptr);
        }
    };

private:
    Node* root = new Node();
    std::vector<std::string> dict;

    bool Find(std::string &s) {
        Node* cur = root;
        for (int i = 0; i < s.size(); ++i) {
            if (cur->v[s[i] - 'a'] == nullptr) {
                return false;
            }
            cur = cur->v[s[i] - 'a'];
        }
        return cur->term != -1;
    }

    bool Add(std::string &s, int id) {
        if (Find(s)) {
            return false;
        }

        Node* cur = root;
        for (int i = 0; i < s.size(); ++i) {
            if (cur->v[s[i] - 'a'] == nullptr) {
                cur->v[s[i] - 'a'] = new Node();
            }
            cur = cur->v[s[i] - 'a'];
        }
        cur->term = id;

        return true;
    }

    void MakeLinks() {
        std::queue<Node*> q;
        std::queue<Node*> p;
        std::queue<int> id;
        q.push(root);
        p.push(nullptr);
        id.push(-1);
        while (!q.empty()) {
            // add children to queue
            Node* cur = q.front();
            Node* par = p.front();
            int idx = id.front();

            q.pop();
            p.pop();
            id.pop();

            for (int i = 0; i < cur->v.size(); ++i) {
                if (cur->v[i] != nullptr) {
                    q.push(cur->v[i]);
                    p.push(cur);
                    id.push(i);
                }
            }
            // make links
            if (par == root || cur == root) {
                cur->f = root;
            } else {
                cur->f = par->f;
                while (cur->f != root && cur->f->v[idx] == nullptr) {
                    cur->f = cur->f->f;
                }
                if (cur->f->v[idx] != nullptr) {
                    cur->f = cur->f->v[idx];
                }

                if (cur->f->term != -1) {
                    cur->e = cur->f;
                } else {
                    cur->e = cur->f->e;
                }
            }
        }
    }

    void CountExitLinks(std::vector<std::vector<int>> &ans, Node* cur, int id) {
        if (cur == root) {
            return;
        }
        if (cur->e != nullptr) {
            cur = cur->e;
            ans[cur->term].push_back(id - dict[cur->term].size() + 1);
            CountExitLinks(ans, cur, id);
        }
    }

public:
    AhoKor(std::vector<std::string> &dict) : dict(dict) {
        for (int i = 0; i < dict.size(); ++i) {
            for (auto &x : dict[i]) {
                if (!(x >= 'a' && x <= 'z')) {
                    throw std::runtime_error("a-z. only.");
                }
            }
            if (dict[i].empty()) {
                throw std::runtime_error("no shitty empty patterns bro...");
            }
            if (!Add(dict[i], i)) {
                throw std::runtime_error("\nYou are banned from this server!\n\nReason: Duping");
            }
        }
        MakeLinks();
    }

    std::vector<std::vector<int>> FindIn(std::string &text) {
        Node* cur = root;

        std::vector<std::vector<int>> ans(dict.size());

        for (int i = 0; i < text.size(); ++i) {
            int c = text[i] - 'a';

            while (cur != root && cur->v[c] == nullptr) {
                cur = cur->f;
            }

            if (cur->v[c] != nullptr) {
                cur = cur->v[c];
                CountExitLinks(ans, cur, i);
            }

            if (cur->term != -1) {
                ans[cur->term].push_back(i - dict[cur->term].size() + 1);
            }
        }

        return ans;
    }
};

int main() {
    // we have a text and pattern with '?' (jokers), answer — all findings of pattern

    std::string text;
    std::string pattern;

    std::cin >> text >> pattern;

    if (pattern.size() > text.size()) {
        return 0;
    }

    std::string cur = "";
    std::map<std::string, std::vector<int>> m;
    int parts = 0;

    std::vector<std::string> dict;

    int i = 0;

    for (auto &x : pattern) {
        if (x == '?') {
            if (cur != "") {
                m[cur].push_back(i - cur.size());
                ++parts;
            }
            cur = "";
        } else {
            cur += x;
        }
        ++i;
    }
    if (cur != "") {
        m[cur].push_back(i - cur.size());
        ++parts;
    }
    int endJoker = 0;
    for (int i = pattern.size() - 1; i >= 0; --i) {
        if (pattern[i] != '?') {
            break;
        } else {
            ++endJoker;
        }
    }

    std::vector<int> ans;

    if (endJoker == pattern.size()) {
        std::cerr << "pattern full of jokers looks strange\n";
        for (int i = 0; i <= text.size() - pattern.size(); ++i) {
            ans.push_back(i);
        }
    } else {
        // main solution
        std::map<int, int> gs;

        std::vector<std::string> dict;
        for (auto [s, v] : m) {
            dict.push_back(s);
        }

        AhoKor ak(dict);
        auto f = ak.FindIn(text);

        for (int i = 0; i < f.size(); ++i) {
            for (auto j : f[i]) {
                for (auto shift : m[dict[i]]) {
                    if (j - shift < 0) {
                        continue;
                    }
                    gs[j - shift]++;
                }
            }
        }

        for (auto [val, c] : gs) {
            if (c == parts && (val + pattern.size() - 1 < text.size())) {
                ans.push_back(val);
            }
        }
    }

    for (auto id : ans) {
        std::cout << id << ' ';
    }
    std::cout << '\n';
}
