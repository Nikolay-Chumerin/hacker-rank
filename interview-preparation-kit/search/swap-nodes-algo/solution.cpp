#include <bits/stdc++.h>

using namespace std;
//------------------------------------------------------------------------------
#define NVERBOSE
#ifdef VERBOSE
#define COUT std::cout
#else
#define COUT \
  while (false) std::cout
#endif
//------------------------------------------------------------------------------
using Type = int;
using Vector = std::vector<Type>;
using Matrix = std::vector<Vector>;

static constexpr int kMaxNodesNum = 1024;
static constexpr int kMaxLevels = kMaxNodesNum;
static constexpr Type kNull = -1;
Vector gResult;
Matrix gIndices;
Vector gLevelSwapped(kMaxLevels + 1);
//------------------------------------------------------------------------------
void traverse_in_order(const int start_idx = 1, int level = 1) {
  COUT << "traverse E: start_idx=" << start_idx << " level=" << level
       << " gIndices.size()=" << gIndices.size();

  const auto& left_son_idx = gIndices[start_idx-1][0];
  const auto& right_son_idx = gIndices[start_idx-1][1];
  COUT << " left_son_idx=" << left_son_idx << " right_son_idx=" << right_son_idx
       << " gLevelSwapped[level]=" << gLevelSwapped[level]
       << " gLevelSwapped.size()=" << gLevelSwapped.size() << "\n";
  if (start_idx == 1) {
    gResult.clear();
  }

  if (gLevelSwapped[level] == 1) {
    if (right_son_idx > 0) {
      COUT << "going right\n";
      traverse_in_order(right_son_idx, level + 1);
    }
    COUT << "pushing " << start_idx << "\n";
    gResult.push_back(start_idx);
    if (left_son_idx > 0) {
      COUT << "going left\n";
      traverse_in_order(left_son_idx, level + 1);
    }
  } else {
    if (left_son_idx > 0) {
      COUT << "going left\n";
      traverse_in_order(left_son_idx, level + 1);
    }
    COUT << "pushing " << start_idx << "\n";
    gResult.push_back(start_idx);
    if (right_son_idx > 0) {
      COUT << "going right\n";
      traverse_in_order(right_son_idx, level + 1);
    }
  }
}  // traverse_in_order(..)
//------------------------------------------------------------------------------
Matrix swap_nodes(const Vector& queries) {
  fill(gLevelSwapped.begin(), gLevelSwapped.end(), 0);
  const auto queries_num = queries.size();
  Matrix result(queries_num);

  for (auto query_idx = 0; query_idx < queries_num; ++query_idx) {
    const auto& k = queries[query_idx];
    for (int i = k; i <= kMaxLevels; i += k) {
      gLevelSwapped[i] = 1 - gLevelSwapped[i];
    }

    traverse_in_order();
    result[query_idx] = gResult;

  }  // loop over queries
  return result;
}
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
Matrix read_matrix() {
  int n;
  cin >> n;
  Matrix matrix(n);
  for (Vector& row_vector : matrix) {
    row_vector.resize(2);
    for (auto& element : row_vector) {
      cin >> element;
    }
  }
  return matrix;
}  // read_matrix()
//------------------------------------------------------------------------------
void save_results(const Matrix& result) {
  const char* output_file_name = getenv("OUTPUT_PATH") ?: "output.txt";
  std::ofstream fout(output_file_name);
  for (int result_row_itr = 0; result_row_itr < result.size();
       result_row_itr++) {
    for (int result_column_itr = 0;
         result_column_itr < result[result_row_itr].size();
         result_column_itr++) {
      fout << result[result_row_itr][result_column_itr];
      if (result_column_itr != result[result_row_itr].size() - 1) {
        fout << " ";
      }
    }
    if (result_row_itr != result.size() - 1) {
      fout << "\n";
    }
  }
  fout << "\n";
  fout.close();
}  // save_result(.)
//------------------------------------------------------------------------------
int main() {
  gIndices = read_matrix();
  const auto queries = read_vector();
  const auto result = swap_nodes(queries);
  save_results(result);
  return 0;
}
