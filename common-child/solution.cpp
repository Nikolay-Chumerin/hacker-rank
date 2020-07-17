#include <bits/stdc++.h>

using namespace std;

#define NVERBOSE
#ifdef VERBOSE
#define COUT cout
#else
#define COUT \
  while (false) cout
#endif

using Index = int;
using Indices = set<Index>;
using IndexMap = array<Indices, 26>;
//------------------------------------------------------------------------------
struct Query {
  Index i1;
  Index i2;
  bool operator==(const Query& other) const {
    return (i1 == other.i1 && i2 == other.i2);
  }
};  // struct Query
//------------------------------------------------------------------------------
struct QueryHash {
  std::size_t operator()(const Query& q) const {
    return ((uint64_t(q.i1) << 13) + q.i2);
  }
};  // struct QueryHash;
//------------------------------------------------------------------------------
using Cache = std::unordered_map<Query, Index, QueryHash>;
//------------------------------------------------------------------------------

class MaxCommonChild {
 public:
  MaxCommonChild(const string& s1, const string& s2) {
    bool s1_mask[26];
    bool s2_mask[26];
    for (auto& v : s1_mask) v = false;
    for (auto& v : s2_mask) v = false;

    for (const auto& c : s1) s1_mask[c-'A'] = true;
    for (const auto& c : s2) s2_mask[c-'A'] = true;
    for (int i = 0; i < 26; ++i)
      s1_mask[i] &= s2_mask[i];

    s1_ = "";
    s2_ = "";

    for (const auto& c : s1) if (s1_mask[c-'A']) s1_.push_back(c);
    for (const auto& c : s2) if (s1_mask[c-'A']) s2_.push_back(c);
    COUT << "s1 ='" << s1 << "'\n";
    COUT << "s1_='" << s1_ << "'\n";
    COUT << "s2 ='" << s2 << "'\n";
    COUT << "s2_='" << s2_ << "'\n";

    l1_ = s1_.size();
    l2_ = s2_.size();
    BuildIndexMap(s2_);
  }  // MaxCommonChild(..)

  int GetMaxCommonChildLen(const Index i1 = 0, const Index i2 = 0) {
    COUT << "Entering GetMaxCommonChildLen(i1=" << i1 << " i2=" << i2 << ") [s1='" << s1_.substr(i1) << "' s2='" << s2_.substr(i2) << "']\n";
    if (i1 < 0 || i2 < 0 || i1 >= l1_ || i2 >= l2_) {
      COUT << "Returning from GetMaxCommonChildLen(i1=" << i1 << " i2=" << i2 << ") -[degenerate]-> 0\n";
      return 0;
    }
    const Query q{i1, i2};
    const auto& cache_it = cache_.find(q);
    if (cache_it != cache_.end()) {
      COUT << "Returning from GetMaxCommonChildLen(i1=" << i1 << " i2=" << i2 << ") -[cache]-> " << cache_it->second << "\n";
      return cache_it->second;
    }
    int max_child_len{0};

    for (Index j1 = i1; j1 < l1_; ++j1) {
      const char& c = s1_[j1];
      const Index idx = c - 'A';
      const Indices& s2_indices = im_[idx];
      // if (s2_indices.empty())
      //   continue;
      auto s2_idx_it = s2_indices.lower_bound(i2);
      // while (s2_idx_it != s2_indices.end()) {
      if (s2_idx_it != s2_indices.end()) {
        Index s2_idx = *s2_idx_it;
        COUT << "s2_idx_it != s2_indices.end()  s2_idx=" << s2_idx << "\n";
        if (s2_idx >= l2_) continue;
        if (c == s2_[s2_idx]) {
          COUT << "++s2_idx: " << s2_idx << " -> " << (1+s2_idx) << "\n";
          ++s2_idx;
        }
        const int candidate = 1 + GetMaxCommonChildLen(j1 + 1, s2_idx);
        if (candidate > max_child_len) {
          max_child_len = candidate;
        }
        // ++s2_idx_it;
      }  // if (s2_idx_it != s2_indices.end())
    }  // j1-loop
    COUT << "Returning from GetMaxCommonChildLen(i1=" << i1 << " i2=" << i2 << ") -[dp]-> " << max_child_len << "\n";
    cache_[q] = max_child_len;
    return max_child_len;
  }  // GetMaxCommonChildLen(..)


 private:
  Cache cache_;
  IndexMap im_;
  string s1_;
  string s2_;
  int l1_;
  int l2_;
  Index max_common_child_len_{0};

  void BuildIndexMap(const string& s) {
    for (int i = 0; i < s.size(); ++i) {
      im_[s[i] - 'A'].insert(i);
    }
  }  // BuildIndexMap()

};  // class MaxCommonChild

int CommonChild(const string& s1, const string& s2) {
  MaxCommonChild mcc(s1, s2);
  return mcc.GetMaxCommonChildLen();
}  // CommonChild(..)

int main() {
  char default_output_file_name[] = "output.txt";
  char* output_file_name = getenv("OUTPUT_PATH");
  if (!output_file_name) output_file_name = default_output_file_name;
  ofstream fout(output_file_name);
  string s1, s2;
  getline(cin, s1);
  getline(cin, s2);
  int result = CommonChild(s1, s2);
  fout << result << "\n";
  fout.close();
  return 0;
}
