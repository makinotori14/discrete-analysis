#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

class ApJa {
// only 'a'-'z' idc
private:
    std::string P;
    std::string Pr;
    int n;

    const int alp = 26;

    std::vector<int> zP;
    std::vector<int> N;

    std::vector<std::vector<int>> bc;
    std::vector<int> gs;
    std::vector<int> gse;

    std::vector<int> M;

    std::vector<int> ZFunc(std::string &s) {
        std::vector<int> z(n);
        if (s.empty()) {
            return {};
        }
        int l = -1;
        int r = -1;
        for (int i = 1; i < n; ++i) {
            if (i < r) {
                int ip = i - l;
                z[i] = z[ip];
                if (z[ip] >= r - i) {
                    int cur = r - i;
                    while (r < n && s[r] == s[cur]) {
                        ++r;
                        ++cur;
                    }
                    l = i;
                    z[i] = r - l;
                }
            } else {
                l = i;
                r = i;
                int cur = 0;
                while (r < n && s[r] == s[cur]) {
                    ++r;
                    ++cur;
                }
                z[i] = r - l;
            }
        }
        return z;
    }

    std::vector<std::vector<int>> BadCharRule() {
        std::vector<std::vector<int>> ans(alp);
        for (int i = 0; i < n; ++i) {
            ans[P[i] - 'a'].push_back(i);
        }
        return ans;
    }

    std::vector<int> GoodSufRule() {
        std::vector<int> ans(n);
        for (int j = 0; j < n - 1; ++j) {
            int k = N[j];
            if (k > 0) {
                ans[k] = n - j - 1;
            }
        }
        return ans;
    }

    std::vector<int> GoodSufExitRule() {
        std::vector<int> ans(n + 1);
        int last = n;
        for (int i = n - 1; i >= 0; --i) {
            ans[n - i] = last;
            if (i > 0 && i + zP[i] == n) {
                last = i;
            }
        }
        return ans;
    }

    int BCShift(char x, int i) {
        int l = -1;
        int r = bc[x - 'a'].size();
        while (r - l > 1) {
            int mid = (l + r) / 2;
            if (bc[x - 'a'][mid] < i) {
                l = mid;
            } else {
                r = mid;
            }
        }
        if (l == -1) {
            return i + 1;
        } else {
            return i - bc[x - 'a'][l];
        }
    }

    void Step(int &l, int &r, std::string &T, int i, int j) {
        int shift = 1;
        shift = std::max(shift, BCShift(T[j], i));
        if (n - i - 1 > 0) {
            if (gs[n - i - 1]) {
                shift = std::max(shift, gs[n - i - 1]);
            } else {
                shift = std::max(shift, gse[n - i - 1]);
            }
        }
        l += shift;
        r += shift;
    }

    void StepMatch(int &l, int &r) {
        l += gse[n];
        r += gse[n];
    }

public:
    ApJa(std::string &P) : P(P), n(P.size()) {
        if (P.empty()) {
            throw std::runtime_error("no empty template");
        }
        Pr = P;
        std::reverse(Pr.begin(), Pr.end());
        zP = ZFunc(P);
        N = ZFunc(Pr);
        std::reverse(N.begin(), N.end());

        bc = BadCharRule();
        gs = GoodSufRule();
        gse = GoodSufExitRule();
    }

    int GetBM(std::string &T) {
        int ans = 0;
        int m = T.size();
        int l = 0;
        int r = l + n - 1;
        while (r < m) {
            int i = n - 1;
            int j = r;
            while (i >= 0 && j >= l && P[i] == T[j]) {
                --i;
                --j;
            }
            if (i < 0) {
                ++ans;
                StepMatch(l, r);
                continue;
            }

            Step(l, r, T, i, j);
        }
        return ans;
    }

    int GetAJ(std::string &T) {
        int ans = 0;
        int m = T.size();
        M.assign(m, -1);

        int l = 0;
        int r = l + n - 1;

        while (r < m) {
            int i = n - 1;
            int j = r;

            while (i >= 0) {
                // undefined or zero-skip
                if (M[j] <= 0) {
                    if (P[i] == T[j]) {
                        if (i == 0) {
                            ++ans;
                            M[r] = n;
                            StepMatch(l, r);
                            break;
                        } else {
                            --i;
                            --j;
                        }
                    } else {
                        M[r] = r - j;
                        Step(l, r, T, i, j);
                        break;
                    }
                } else if (M[j] > N[i]) {
                    if (N[i] < i + 1) {
                        M[r] = r - j;
                        j -= N[i];
                        i -= N[i];
                        Step(l, r, T, i, j);
                        break;
                    } else {
                        ++ans;
                        M[r] = r - j;
                        StepMatch(l, r);
                        break;
                    }
                } else if (M[j] == N[i]) {
                    if (N[i] == i + 1) {
                        ++ans;
                        M[r] = r - j;
                        StepMatch(l, r);
                        break;
                    }
                    j -= N[i];
                    i -= N[i];
                } else if (M[j] < N[i]) {
                    i -= M[j];
                    j -= M[j];
                }
            }
        }

        return ans;
    }
};