#include <bits/stdc++.h>
using namespace std;
using Strings = vector<string>;
using Matrix = vector<vector<int>>;

//------------------------------------------------------------------------------
Strings split_string(string input_string) {
  string::iterator new_end =
      unique(input_string.begin(), input_string.end(),
             [](const char &x, const char &y) { return x == y and x == ' '; });
  input_string.erase(new_end, input_string.end());
  while (input_string[input_string.length() - 1] == ' ') {
    input_string.pop_back();
  }
  Strings splits;
  char delimiter = ' ';
  size_t i = 0;
  size_t pos = input_string.find(delimiter);
  while (pos != string::npos) {
    splits.push_back(input_string.substr(i, pos - i));
    i = pos + 1;
    pos = input_string.find(delimiter, i);
  }
  splits.push_back(
      input_string.substr(i, min(pos, input_string.length()) - i + 1));
  return splits;
}  // split_string(.)
//------------------------------------------------------------------------------
enum CellTypes {
  kEmpty = -3,
  kBlocked = -2,
  kGoal = -1,
  kStart = 0
};  // enum CellTypes
//------------------------------------------------------------------------------
class Field {
 public:
  Field(const Strings &grid, const int start_x, const int start_y,
        const int goal_x, const int goal_y)
      : start_x_(start_x), start_y_(start_y), goal_x_(goal_x), goal_y_(goal_y) {
    n_ = grid.size();
    // cout << "goal_x_=" << goal_x_ << " goal_y_=" << goal_y_ << "\n";
    InitField();
    SetStart();
    SetGoal();
    SetBlockedCells(grid);
  };  // ctr
  //------------------------------------------------------------------------------
  void InitField() {
    field_.resize(n_ + 2);
    for (auto &row : field_) {
      row.resize(n_ + 2);
      fill(row.begin(), row.end(), kEmpty);
      row.front() = kBlocked;
      row.back() = kBlocked;
    }
    auto &first_row = field_.front();
    fill(first_row.begin(), first_row.end(), kBlocked);

    auto &last_row = field_.back();
    fill(last_row.begin(), last_row.end(), kBlocked);
  };  // InitField()
  //------------------------------------------------------------------------------
  void SetStart() {
    field_[start_y_ + 1][start_x_ + 1] = kStart;
  };  // SetStart()
  //------------------------------------------------------------------------------
  void SetGoal() { field_[goal_y_ + 1][goal_x_ + 1] = kGoal; };  // SetGoal()
  //------------------------------------------------------------------------------
  void SetBlockedCells(const Strings &grid) {
    for (int grid_row_idx = 0; grid_row_idx < n_; ++grid_row_idx) {
      const auto &row_str = grid[grid_row_idx];
      const int field_row_idx = grid_row_idx + 1;
      for (int grid_col_idx = 0; grid_col_idx < n_; ++grid_col_idx) {
        const int field_col_idx = grid_col_idx + 1;
        if (row_str[grid_col_idx] == 'X')
          field_[field_row_idx][field_col_idx] = kBlocked;
      }  // loop over grid columns
    }    // loop over grid rows
  };     // SetBlockedCells(.)
  //------------------------------------------------------------------------------
  void Print() {
    cout << "\n";
    for (const auto &row : field_) {
      for (const auto &el : row) {
        switch (el) {
          case kBlocked:
            cout << "X";
            break;
          case kStart:
            cout << "S";
            break;
          case kGoal:
            cout << "G";
            break;
          case kEmpty:
            cout << ".";
            break;
          default:
            cout << el;
            break;
        }  // switch
      }    // loop over row elements
      cout << "\n";
    }  // loop over field_ rows
  }    // Print()
  //------------------------------------------------------------------------------
  int Fill(const int row_idx, const int col_idx, const int row_inc,
           const int col_inc, const int value) {
    int r = row_idx + row_inc;
    int c = col_idx + col_inc;
    int changed_cells_num{0};

    while (field_[r][c] != kBlocked) {
      if (field_[r][c] == kEmpty || field_[r][c] == kGoal ||
          field_[r][c] > value) {
        field_[r][c] = value;
        ++changed_cells_num;
      }
      r += row_inc;
      c += col_inc;
    };
    return changed_cells_num;
  }  // Fill(.....)
  //------------------------------------------------------------------------------
  int GetStepsNum() {
    if (goal_x_ == start_x_ && goal_y_ == start_y_) return 0;
    auto &goal_cell_value = field_[goal_y_ + 1][goal_x_ + 1];
    int changed_cells_num = 1;
    while (changed_cells_num) {
      changed_cells_num = 0;
      for (int row_idx = 1; row_idx <= n_; ++row_idx) {
        auto &row = field_[row_idx];
        for (int col_idx = 1; col_idx <= n_; ++col_idx) {
          auto &val = row[col_idx];
          if (val >= kStart) {
            changed_cells_num +=
                Fill(row_idx, col_idx, 0, +1, val + 1);  // fill right
            changed_cells_num +=
                Fill(row_idx, col_idx, 0, -1, val + 1);  // fill left
            changed_cells_num +=
                Fill(row_idx, col_idx, +1, 0, val + 1);  // fill down
            changed_cells_num +=
                Fill(row_idx, col_idx, -1, 0, val + 1);  // fill up
            // Print();
          }
        }  // col loop
      }    // row loop
    }      // main loop
    return goal_cell_value;
  }  // int GetStepsNum()
  //------------------------------------------------------------------------------

  int n_;
  int goal_x_;
  int goal_y_;
  int start_x_;
  int start_y_;
  Matrix field_;
};  // class Field
//------------------------------------------------------------------------------
int MinimumMoves(const Strings &grid, const int start_row, const int start_col,
                 const int goal_row, const int goal_col) {
  auto field = Field(grid, start_col, start_row, goal_col, goal_row);
  // field.Print();
  const int result = field.GetStepsNum();
  // cout << "----------------\n";
  // field.Print();
  return result;
}  // MinimumMoves(.....)
//------------------------------------------------------------------------------
int main() {
  char default_output_file_name[] = "output.txt";
  char *output_file_name = getenv("OUTPUT_PATH");
  if (!output_file_name) output_file_name = default_output_file_name;
  ofstream fout(output_file_name);

  int n;
  cin >> n;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  Strings grid(n);
  for (int i = 0; i < n; i++) {
    string grid_item;
    getline(cin, grid_item);
    grid[i] = grid_item;
  }
  string startXStartY_temp;
  getline(cin, startXStartY_temp);
  const auto startXStartY = split_string(startXStartY_temp);
  int startX = stoi(startXStartY[0]);
  int startY = stoi(startXStartY[1]);
  int goalX = stoi(startXStartY[2]);
  int goalY = stoi(startXStartY[3]);
  int result = MinimumMoves(grid, startX, startY, goalX, goalY);
  fout << result << "\n";
  fout.close();
  return 0;
}  // main()
