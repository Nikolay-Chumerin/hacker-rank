#include <bits/stdc++.h>
//------------------------------------------------------------------------------
using Type = unsigned int;
using Vector = std::vector<Type>;
using Set = std::set<Type, std::greater<Type>>;
//------------------------------------------------------------------------------
static constexpr int kTypeBitsNum = std::numeric_limits<Type>::digits;
static constexpr Type kHighestBitOnly = (1 << (kTypeBitsNum - 1));
//------------------------------------------------------------------------------
class TrieNode {
 public:
  TrieNode* zero_{nullptr};
  TrieNode* one_{nullptr};
};  // class TrieNode
//------------------------------------------------------------------------------
class Trie {
  TrieNode* root_{nullptr};

 public:
  Trie(const Set& values) {
    root_ = new TrieNode();
    for (const auto& v : values) {
      Insert(v);
    }
  };  // ctr
  //----------------------------------------------------------------------------
  ~Trie() { Delete(root_); };  // dtr
  //----------------------------------------------------------------------------
  void Delete(TrieNode* node) {
    if (!node) return;  // sanity check
    if (node->one_) {
      Delete(node->one_);
      node->one_ = nullptr;
    }
    if (node->zero_) {
      Delete(node->zero_);
      node->zero_ = nullptr;
    }
  }  // Delete(.)
  //----------------------------------------------------------------------------
  void Insert(const Type x) {
    Type bit_mask{kHighestBitOnly};
    TrieNode* curr{root_};
    while (bit_mask) {
      if (bit_mask & x) {
        if (!curr->one_) curr->one_ = new TrieNode();
        curr = curr->one_;
      } else {
        if (!curr->zero_) curr->zero_ = new TrieNode();
        curr = curr->zero_;
      }
      bit_mask >>= 1;
    }
  };  // Insert(.)
  //----------------------------------------------------------------------------
  Type GetClosestMatch(const Type x) {
    Type result{0};
    Type bit_mask{kHighestBitOnly};
    TrieNode* curr{root_};
    while (bit_mask) {
      if (bit_mask & x) {
        // try "one" direction first
        if (curr->one_) {
          result |= bit_mask;
          curr = curr->one_;
        } else {
          if (curr->zero_) curr = curr->zero_;
        }
      } else {
        // try "zero" direction first
        if (curr->zero_) {
          curr = curr->zero_;
        } else {
          if (curr->one_) {
            result |= bit_mask;
            curr = curr->one_;
          }
        }
      }
      bit_mask >>= 1;
    }  // while (bit_mask)
    return result;
  };  // GetClosestMatch(.)
};    // class Trie
//------------------------------------------------------------------------------
Vector read_vector() {
  int m;
  std::cin >> m;
  Vector vec(m);
  for (auto& el : vec) {
    std::cin >> el;
  }
  return vec;
}  // read_vector()
//------------------------------------------------------------------------------
Set get_unique_values(const Vector& vec) {
  Set s;
  for (const auto& x : vec) {
    s.insert(x);
  }
  return s;
}  // get_unique_values()
//------------------------------------------------------------------------------
Vector get_max_xors(const Set& values, const Vector& queries) {
  Trie trie(values);
  const auto queries_num = queries.size();
  Vector result(queries_num);
  for (auto i = 0; i < queries_num; ++i) {
    const auto& q = queries[i];
    const auto inverted_q = (kHighestBitOnly - 1) ^ q;
    const auto best_match = trie.GetClosestMatch(inverted_q);
    result[i] = q ^ best_match;
  }  // main loop
  return result;
}  // get_max_xors(..)
//------------------------------------------------------------------------------
void save_results(const Vector& vec) {
  const char* output_file_name = getenv("OUTPUT_PATH") ?: "output.txt";
  std::ofstream fout(output_file_name);
  for (const auto& el : vec) {
    fout << el << "\n";
  }
  fout.close();
}  // save_result(.)
//------------------------------------------------------------------------------
int main() {
  const auto values = get_unique_values(read_vector());
  const auto queries = read_vector();
  const auto results = get_max_xors(values, queries);
  save_results(results);
  return 0;
}
