#include <bits/stdc++.h>

using namespace std;

#define VERBOSE
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
  }  // operator()
};  // struct QueryHash;
//------------------------------------------------------------------------------
using Cache = std::unordered_map<Query, Index, QueryHash>;
using LetterSet = array<bool, 26>;
//------------------------------------------------------------------------------
LetterSet GetSetOfCommonLetters(const string& s1, const string& s2) {
  LetterSet s1_set, s2_set, result;
  s1_set.fill(false);
  s2_set.fill(false);
  result.fill(false);
  for (const auto& c : s1) s1_set[c-'A'] = true;
  for (const auto& c : s2) s2_set[c-'A'] = true;
  for (int i = 0; i < result.size(); ++i)
    result[i] = s1_set[i] && s2_set[i];
  return result;
}  // GetSetOfCommonLetters(..)
//------------------------------------------------------------------------------
string FilterString(const string& s, const LetterSet& filter_set) {
  string result;
  for (const char& c : s)
    if (filter_set[c - 'A'])
      result.push_back(c);
  return result;
}  // FilterString(..)
//------------------------------------------------------------------------------
class MaxCommonChild {
 public:
  MaxCommonChild(const string& s1, const string& s2) {
    const LetterSet common_letters = GetSetOfCommonLetters(s1, s2);
    s1_ = FilterString(s1, common_letters);
    s2_ = FilterString(s2, common_letters);
    l1_ = s1_.size();
    l2_ = s2_.size();

    COUT << "s1 ='" << s1 << "'\n";
    COUT << "s1_='" << s1_ << "' l1_=" << l1_ << "\n";
    COUT << "s2 ='" << s2 << "'\n";
    COUT << "s2_='" << s2_ << "' l2_=" << l2_ << "\n";
 
    BuildIndexMap(s2_);
  }  // MaxCommonChild(..)


  int GetMaxCommonChildLen(const Index i1 = 0, const Index i2 = 0 /*, const Index common_letters_num = 0*/) {
    COUT << "Entering GetMaxCommonChildLen(i1=" << i1 << " i2=" << i2 << ")";
    // COUT << " [s1='" << s1_.substr(i1) << "' s2='" << s2_.substr(i2) << "']";
    COUT << "\n";
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
        // if (c == s2_[s2_idx]) {
        //   COUT << "++s2_idx: " << s2_idx << " -> " << (1+s2_idx) << "\n";
        //   ++s2_idx;
        // }
        const int candidate = 1 + GetMaxCommonChildLen(j1 + 1, s2_idx + 1/*, common_letters_num + 1*/);
        if (candidate > max_child_len) {
          max_child_len = candidate;
        }
        // if (common_letters_num + max_child_len > max_common_child_len_)
        //   max_common_child_len_ = common_letters_num + max_child_len;
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
