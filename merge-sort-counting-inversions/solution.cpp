#include <bits/stdc++.h>

using namespace std;

using Size = long;
using Count = Size;
using Index = int;
using Value = int;
using Values = vector<Value>;
static constexpr Value kMaxValue = 10'000'000;
static constexpr Size kTreeLeavesNum = 16'777'216;
static constexpr Size kTreeLength = 2 * kTreeLeavesNum;
using SegmentTree = array<Count, kTreeLength>;
SegmentTree tree;
//------------------------------------------------------------------------------
Size next_power_of_2(Size n) {
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n++;
  return n;
}  // next_power_of_2(.)
//------------------------------------------------------------------------------
Count CountInversionsNaive(Values& v) {
  const Size n = v.size();
  Count res{0};
  for (Index i = 0; i < n - 1; ++i) {
    const Value& a = v[i];
    for (Index j = i + 1; j < n; ++j) {
      if (a > v[j]) ++res;
    }  // j-loop
  }    // i-loop
  return res;
}  // CountInversionsNaive(.)
//------------------------------------------------------------------------------
/*
0 1 2 3 4 5 6 7

2 1 3 1 2 0 0 0
0 0 0 0 0 0 0 0
 1   1   0   0

Initial tree:
0[0] 0[1] 0[2] 0[3] 0[4] 0[5] 0[6] 0[7]
   0[0:1]     0[2:3]    0[4:5]    0[6:7]
         0[0:3]               0[4:7]
                    0[0:7]

insert 2
0[0] 0[1] 1[2] 0[3] 0[4] 0[5] 0[6] 0[7]
   0[0:1]     1[2:3]    0[4:5]    0[6:7]
         1[0:3]               0[4:7]
                    1[0:7]
total number of inversions: 0 + 0 = 0

insert 1
0[0] 1[1] 1[2] 0[3] 0[4] 0[5] 0[6] 0[7]
   1[0:1]     1[2:3]    0[4:5]    0[6:7]
         2[0:3]               0[4:7]
                    2[0:7]
total number of inversions: 0 + 1 = 1

insert 3
0[0] 1[1] 1[2] 1[3] 0[4] 0[5] 0[6] 0[7]
   1[0:1]     2[2:3]    0[4:5]    0[6:7]
         3[0:3]               0[4:7]
                    3[0:7]
total number of inversions: 1 + 0 = 1

insert 1
0[0] 2[1] 1[2] 1[3] 0[4] 0[5] 0[6] 0[7]
   2[0:1]     2[2:3]    0[4:5]    0[6:7]
         4[0:3]               0[4:7]
                    4[0:7]
total number of inversions: 1 + 2 = 3

insert 2
0[0] 2[1] 2[2] 1[3] 0[4] 0[5] 0[6] 0[7]
   2[0:1]     3[2:3]    0[4:5]    0[6:7]
         5[0:3]               0[4:7]
                    5[0:7]
total number of inversions: 3 + 1 = 4


if going to the left son:
  increase number of inversions by the value in the right son
*/

Count CountInversions(const Values& v) {
  Count res{0};
  tree.fill(0);
  for (const auto& a : v) {
    // insert a into the tree
    Index i = kTreeLeavesNum + a;
    while (i) {
      ++tree[i];             // increase number of elements in the current node
      if (!(i & 1)) {        // i - is the left son?
        res += tree[i + 1];  // then the right brother has number of elements
                             // greater then a - all of which are the inversions
                             // w.r.t. a
      }
      i >>= 1;  // go one level up to the root
    }           // tree-traverse loop
  }             // loop over elements of v
  return res;
}  // CountInversions(.)
//------------------------------------------------------------------------------
int main() {
  char default_output_file_name[] = "output.txt";
  char* output_file_name = getenv("OUTPUT_PATH");
  if (!output_file_name) output_file_name = default_output_file_name;
  ofstream fout(output_file_name);

  int t, n;
  cin >> t;

  for (int test_idx = 0; test_idx < t; ++test_idx) {
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
      cin >> arr[i];
    }
    Count result = CountInversions(arr);
    fout << result << "\n";
  }
  fout.close();
  return 0;
}
