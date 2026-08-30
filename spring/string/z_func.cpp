#include <string>
#include <vector>

class ZFunc {
private:
    std::string s;
    std::vector<int> z;

    void Build() {
        if (z.empty()) {
            return;
        }
        z[0] = 0;
        int l = -1, r = -1;
        for (int i = 1; i < z.size(); ++i) {
            if (i < r) {
                int idx = i - l;
                int rem_size = r - i;
                if (z[idx] < rem_size) {
                    z[i] = z[idx];
                } else {
                    int cur = r - i;
                    while (r < s.size() && s[r] == s[cur]) {
                        ++cur;
                        ++r;
                    }
                    l = i;
                    z[i] = r - l;
                }
            } else {
                l = i;
                r = i;
                int cur = 0;
                while (r < s.size() && s[r] == s[cur]) {
                    ++cur;
                    ++r;
                }
                z[i] = r - l;
            }
        }
    }

public:
    ZFunc(std::string s) : s(s), z(s.size()) {
        Build();
    }
};