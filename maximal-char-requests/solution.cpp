#include <bits/stdc++.h>
using namespace std;

#define NO_VERBOSE
#ifdef VERBOSE
#define COUT cout
#else
#define COUT \
  while (false) cout
#endif

using Index = unsigned int;
using Count = unsigned int;
//------------------------------------------------------------------------------
struct Query {
  Index first;
  Index last;
  bool operator==(const Query& other) const {
    return (first == other.first && last == other.last);
  }
};  // struct Query
//------------------------------------------------------------------------------
struct QueryHash {
  std::size_t operator()(const Query& q) const {
    return ((uint64_t(q.first) << 20) + q.last);
  }
};  // struct QueryHash;
//------------------------------------------------------------------------------
using Cache = std::unordered_map<Query, Count, QueryHash>;
using Queries = std::vector<Query>;

//------------------------------------------------------------------------------

struct IntervalResult {          // result for come semi-closed interval (i,j]
  Count greatest_char_count{0};  // 32
  Index next_qpoint{0};          // 32
  char greatest_char{0};         // 8
  char padding[7];               // 56 padding to 128 bit;
};                               // struct IntervalResult
//------------------------------------------------------------------------------
struct SegmentData {
  char ch : 8;
  unsigned int count : 24;
};  // struct SegmentData
//------------------------------------------------------------------------------
using SegmentTree = vector<SegmentData>;

static constexpr Index kMaxStringLength = 300'000;
static constexpr Index kMaxQueriesNum = 100'000;
static constexpr Index kMaxIndex = 16'777'216 - 1;  // 2^24 - 1
static constexpr SegmentData kDefaultSegment{0, 0U};

Cache cache;
vector<Count> result(kMaxQueriesNum);
set<Index> qpoints;
IntervalResult interval_results[kMaxStringLength];
//------------------------------------------------------------------------------
void GetMaxCharCountDirect(const string& s, const Queries& queries) {
  const Index queries_num = queries.size();
  for (Index q_idx = 0; q_idx < queries_num; ++q_idx) {
    const Query& query{queries[q_idx]};
    int greatest_char_frequency{1};
    if (query.last > query.first) {
      greatest_char_frequency = cache[query];
      if (greatest_char_frequency == 0) {
        ++greatest_char_frequency;
        char greatest_char = s[query.last];
        for (int i = query.first; i < query.last; ++i) {
          const char& c = s[i];
          if (c > greatest_char) {
            greatest_char = c;
            greatest_char_frequency = 1;
          } else if (c == greatest_char) {
            ++greatest_char_frequency;
          }
        }  // loop over chars in the interval
        cache[query] = greatest_char_frequency;
      }  // cache check
    }    // if (query.last > query.first)
    result[q_idx] = greatest_char_frequency;
  }  // loop over queries
}  // GetMaxCharCountDirect(..)

//------------------------------------------------------------------------------
void GetMaxCharCountPrev(const string& s, const Queries& queries) {
  COUT << "s='" << s << "'\n";
  const Count queries_num = queries.size();
  Index max_qpoint;

  COUT << "Queries = [ ";
  for (const auto& q : queries) {
    COUT << "(" << q.first << ", " << q.last << ") ";
    qpoints.insert(q.first);
    qpoints.insert(q.last);
  }  // loop over queries
  max_qpoint = *qpoints.rbegin();
  COUT << "]\n";
  const Count qpoints_num = qpoints.size();

  Index first{0}, last{max_qpoint + 1}, qpoint_idx{0};
  char greatest_char{' '};
  Count greatest_char_count{1};
  COUT << "qpoints = [ ";
  for (const auto& qpoint : qpoints) {
    COUT << qpoint << " (f=" << first << " l=" << last << " -> ";
    first = last;
    last = qpoint;
    COUT << "f=" << first << " l=" << last << ") ";

    if (first <= max_qpoint) {
      greatest_char = s[first];
      greatest_char_count = 1;
      for (int i = first + 1; i <= last; ++i) {
        const char& c = s[i];
        if (c > greatest_char) {
          greatest_char = c;
          greatest_char_count = 1;
        } else if (c == greatest_char) {
          ++greatest_char_count;
        }
      }  // loop over chars in the interval
      interval_results[first].greatest_char_count = greatest_char_count;
      interval_results[first].next_qpoint = last;
      interval_results[first].greatest_char = greatest_char;
    }  // if not first iteration
  }    // for (const auto& qpoint : qpoints)
  COUT << "]\n";
  COUT << "Intervals:\n";
  for (Index i = 0; i <= max_qpoint; ++i) {
    COUT << "\ti=" << i << " char='" << interval_results[i].greatest_char
         << "' ";
    COUT << "count=" << interval_results[i].greatest_char_count << " ";
    COUT << "next=" << interval_results[i].next_qpoint << "\n";
  }

  for (int q_idx = 0; q_idx < queries_num; ++q_idx) {
    const Query& query{queries[q_idx]};
    COUT << "Query #" << q_idx << ": first=" << query.first
         << " last=" << query.last;
    COUT << " s[]='" << s.substr(query.first, query.last - query.first + 1)
         << "'\n";

    Count greatest_char_count = cache[query];
    if (greatest_char_count == 0) {
      ++greatest_char_count;
      Index first = query.first;
      char greatest_char{' '};
      while (first < query.last) {
        COUT << "  first=" << first << " greatest_char='" << greatest_char
             << "' greatest_char_count=" << greatest_char_count << "\n";
        if (greatest_char == interval_results[first].greatest_char) {
          greatest_char_count += interval_results[first].greatest_char_count;
          if (greatest_char == s[first]) --greatest_char_count;

        } else {
          if (greatest_char < interval_results[first].greatest_char) {
            greatest_char_count = interval_results[first].greatest_char_count;
            greatest_char = interval_results[first].greatest_char;
          }
        }
        first = interval_results[first].next_qpoint;
        COUT << "  first=" << first << " greatest_char='" << greatest_char
             << "' greatest_char_count=" << greatest_char_count << "\n\n";
        if (first == 0) break;
      }  // while (first < query.last)
      cache[query] = greatest_char_count;
    }  // cache check
    result[q_idx] = greatest_char_count;
  }  // loop over queries
}  // GetMaxCharCountPrev(..)

//------------------------------------------------------------------------------
SegmentData MergeSegments(const SegmentData& left, const SegmentData& right) {
  SegmentData res{left};
  if (res.ch == right.ch) {
    res.count += right.count;
  } else if (res.ch < right.ch) {
    res = right;
  }
  return res;
}  // MergeSegments

//------------------------------------------------------------------------------
void BuildSegmentTree(const string& s, SegmentTree& seg_tree) {
  const Count string_len = s.size();
  const Count n = (1 << int(log2(string_len - 1) + 1));
  COUT << "string_len=" << string_len << " n=" << n << "\n";
  seg_tree.resize(2 * n, kDefaultSegment);

  // Build the segment tree
  for (int i = 0; i < string_len; ++i)
    seg_tree[n + i] = {s[i], 1U};  // init leaves
  // build the rest of the tree
  for (int i = n - 1; i > 0; --i) {
    const auto& left_son = seg_tree[2 * i];
    const auto& right_son = seg_tree[2 * i + 1];
    seg_tree[i] = MergeSegments(left_son, right_son);
  }  // loop over the rest of the tree
}  // BuildSegmentTree()

//------------------------------------------------------------------------------
Count SearchTreeUp(const SegmentTree& st, Index l, Index r) {
  SegmentData ans{0, 0};
  const Count n = st.size() / 2;
  l += n - 1;
  r += n - 1;
  while (l <= r) {
    if (l & 1) ans = MergeSegments(st[l], ans);
    if (!(r & 1)) ans = MergeSegments(ans, st[r]);
    l = (l + 1) / 2;
    r = (r - 1) / 2;
  }  // while (l <= r)
  return ans.count;
}  // SearchTreeUp(...)

//------------------------------------------------------------------------------
void GetMaxCharCountUsingSegmentTree(const string& s, const Queries& queries) {
  SegmentTree seg_tree;
  BuildSegmentTree(s, seg_tree);
  const Count string_len = s.size();
  const Count queries_num = queries.size();
  for (int q_idx = 0; q_idx < queries_num; ++q_idx) {
    const Query& query{queries[q_idx]};
    Count greatest_char_count = cache[query];
    if (greatest_char_count == 0) {
      ++greatest_char_count;
      greatest_char_count =
          SearchTreeUp(seg_tree, query.first + 1, query.last + 1);
      cache[query] = greatest_char_count;
    }  // cache check
    result[q_idx] = greatest_char_count;
  }  // loop over queries
}  // GetMaxCharCountUsingSegmentTree(..)

//------------------------------------------------------------------------------
void ToUpperCase(string& s) {
  for (char& c : s) c &= 0xDF;
}  // ToUpperCase(string& s)

//------------------------------------------------------------------------------
int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  int n;
  int q;
  string s;
  cin >> n >> s >> q;
  ToUpperCase(s);
  Queries queries(q);
  for (auto& query : queries) {
    cin >> query.first >> query.last;
  }

  GetMaxCharCountUsingSegmentTree(s, queries);
  for (int i = 0; i < q; i++) {
    fout << result[i];
    if (i != q - 1) {
      fout << "\n";
    }
  }
  fout << "\n";
  fout.close();

  return 0;
}