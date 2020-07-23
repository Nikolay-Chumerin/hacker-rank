#include <bits/stdc++.h>
using namespace std;

#define NVERBOSE
#ifdef VERBOSE
#define COUT cout
#else
#define COUT \
  while (false) cout
#endif


static constexpr int kSize = 10;
using Strings = vector<string>;
using FieldRow = array<char, kSize + 2>;
using Field = array<FieldRow, kSize + 2>;
enum Orientation { kHorizontal = 0, kVertical };
struct Placeholder {
  int row, col, len;
  Orientation orientation;
  int word_idx;
  Placeholder(const int r, const int c, const int l, const Orientation o,
              const int wi = -1)
      : row(r), col(c), len(l), orientation(o), word_idx(wi) {}
};  // Placeholder
using Placeholders = vector<Placeholder>;
//------------------------------------------------------------------------------
Strings Split(const string& s, const char delimiter) {
  Strings tokens;
  string token;
  istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}  // Split(..)
//------------------------------------------------------------------------------
Field MakeField(const Strings& crossword) {
  Field field;
  field.back().fill('+');
  field.front().fill('+');
  for (int row_idx = 1; row_idx <= kSize; ++row_idx) {
    const string& row_str = crossword[row_idx - 1];
    FieldRow& row = field[row_idx];
    row.front() = '+';
    row.back() = '+';
    for (int col_idx = 1; col_idx <= kSize; ++col_idx) {
      row[col_idx] = row_str[col_idx - 1];
    }  // col_idx-loop
  }    // row_idx-loop
  return field;
}  // MakeField(.)
//------------------------------------------------------------------------------
Placeholders GetPlaceholders(const Field& field) {
  Placeholders placeholders;
  for (int r = 1; r <= kSize; ++r) {
    for (int c = 1; c <= kSize; ++c) {
      if (field[r][c] == '-' && field[r - 1][c] != '-' &&
          field[r + 1][c] == '-') {
        int len{2};
        while (field[r + len][c] == '-') ++len;
        placeholders.emplace_back(r, c, len, kVertical);
      }  // found vertical placefolder
      if (field[r][c] == '-' && field[r][c - 1] != '-' &&
          field[r][c + 1] == '-') {
        int len{2};
        while (field[r][c + len] == '-') ++len;
        placeholders.emplace_back(r, c, len, kHorizontal);
      }  // found vertical placefolder
    }    // loop over cols
  }      // loop over rows
  return placeholders;
}  // GetPlaceholders(.)
//------------------------------------------------------------------------------
void PrintPlaceholders(const Placeholders& placeholders) {
  COUT << "Placeholders ------------------------------------------\n";
  for (const auto& placeholder : placeholders) {
    COUT << "row=" << placeholder.row << " col=" << placeholder.col;
    COUT << " len=" << placeholder.len << " orientation=";
    COUT << (placeholder.orientation == kVertical ? "ver" : "hor");
    COUT << " word_idx=" << placeholder.word_idx;
    COUT << "\n";
  }  // loop over placeholders
}  // PrintPlaceholders(.)
//------------------------------------------------------------------------------
void PrintField(const Field& field) {
  COUT << "Field:-----------------\n";
  for (int row_idx = 1; row_idx <= kSize; ++row_idx) {
    for (int col_idx = 1; col_idx <= kSize; ++col_idx) {
      COUT << field[row_idx][col_idx];
    }  // loop over cols
    COUT << "\n";
  }  // loop over rows
  COUT << "\n";
}  //  PrintField(.)
//------------------------------------------------------------------------------
bool CheckWordFit(const Placeholders& placeholders, const Strings& words,
                  const Field& field) {
  COUT << "\tEntering CheckWordFit(...)\n";
  const int words_num = words.size();
  for (const auto& placeholder : placeholders) {
    const auto& wi = placeholder.word_idx;
    if (wi < 0) continue;
    if (wi >= words_num || words[wi].size() != placeholder.len) {
      return false;
    }  // length check
  }    // loop over placeholders
  COUT << "\t\tlengths are Ok.\n";
  // fit check
  Field filled_field = field;
  for (const auto& p : placeholders) {
    const auto& wi = p.word_idx;
    if (wi < 0) continue;
    const string& word = words[wi];
    int row = p.row;
    int col = p.col;
    int row_inc = (p.orientation == kVertical);
    int col_inc = 1 - row_inc;
    COUT << "p.orientation=" << p.orientation << " row_inc=" << row_inc << " col_inc=" << col_inc << "\n";
    for (int i = 0; i < p.len; ++i) {
      if (filled_field[row][col] == '-' || filled_field[row][col] == toupper(word[i])) {
        filled_field[row][col] = toupper(word[i]);
        row += row_inc;
        col += col_inc;
      } else {
        PrintField(filled_field);
        return false;
      }
    }  // loop ower word chars
  }    // loop over placeholders
  return true;
}  // CheckWordFit(...)
//------------------------------------------------------------------------------
Field FillField(const Placeholders& placeholders, const Strings& words,
                  const Field& field) {
  const int words_num = words.size();

  Field filled_field = field;
  for (const auto& p : placeholders) {
    const auto& wi = p.word_idx;
    if (wi < 0) continue;
    const string& word = words[wi];
    int row = p.row;
    int col = p.col;
    int row_inc = (p.orientation == kVertical);
    int col_inc = 1 - row_inc;
    for (int i = 0; i < p.len; ++i) {
        filled_field[row][col] = toupper(word[i]);
        row += row_inc;
        col += col_inc;
    }  // loop ower word chars
  }    // loop over placeholders
  return filled_field;
}  // FillField(...)
//------------------------------------------------------------------------------
void SaveField(const Field& field, const char* output_file_name) {
  ofstream fout(output_file_name);
  for (int row_idx = 1; row_idx <= kSize; ++row_idx) {
    for (int col_idx = 1; col_idx <= kSize; ++col_idx) {
      fout << field[row_idx][col_idx];
    }  // loop over cols
    fout << "\n";
  }  // loop over rows
  fout.close();
}  // SaveField(..)
//------------------------------------------------------------------------------
Strings ReadCrossword() {
  Strings crossword_strings(kSize);
  for (auto& crossword_string : crossword_strings) {
    getline(cin, crossword_string);
  }
  return crossword_strings;
}  // ReadCrossword()
//------------------------------------------------------------------------------
bool CanBeSolved(Field& field, Strings& words, Placeholders& placeholders,
                 int ph_idx = 0) {
  COUT << "CanBeSolved: ph_idx=" << ph_idx << "\n";
  if (ph_idx >= placeholders.size()) return true;

  auto& ph = placeholders[ph_idx];
  COUT << "Checking placeholder#" << ph_idx << " row=" << ph.row << " col=" << ph.col << " len=" << ph.len  << "\n";
  int l = ph.len;
  for (int word_idx = 0; word_idx < words.size(); ++word_idx) {
    auto& word = words[word_idx];
    COUT << "\tword=" << word << " length must be " << l << "\n";
    if (word[0] <= 'Z' && word.size() == l) {
      ph.word_idx = word_idx;
      bool fit = CheckWordFit(placeholders, words, field);
      COUT << "\tfit=" << fit << "(" << word << ")\n";
      if (fit) {
        word[0] = tolower(word[0]);
        bool possible = CanBeSolved(field, words, placeholders, ph_idx + 1);
        if (possible) {
          return true;
        } else {
          word[0] = toupper(word[0]);
          ph.word_idx = -1;
        }  // if possible
      } else {
        ph.word_idx = -1;
      }  // if (fit)
    }    // if (word[0] <= 'Z' && word.size() == l)
  }      // loop over words
  COUT << "\treturning false\n";
  return false;
}  // CanBeSolved(....)

//------------------------------------------------------------------------------
Field SolveCrosswordPuzzle(Field& field, Strings& words,
                           Placeholders& placeholders, int ph_idx = 0) {
  PrintPlaceholders(placeholders);
  bool possible = CanBeSolved(field, words, placeholders, ph_idx);
  PrintPlaceholders(placeholders);
  return FillField(placeholders, words, field);
}  // SolveCrosswordPuzzle()
//------------------------------------------------------------------------------
int main() {
  char default_output_file_name[] = "output.txt";
  char* output_file_name = getenv("OUTPUT_PATH");
  if (!output_file_name) output_file_name = default_output_file_name;
  Strings crossword_strings = ReadCrossword();
  Field field = MakeField(crossword_strings);
  PrintField(field);
  Placeholders placeholders = GetPlaceholders(field);
  PrintPlaceholders(placeholders);

  string words_string;
  getline(cin, words_string);
  auto words = Split(words_string, ';');
  auto solution_field = SolveCrosswordPuzzle(field, words, placeholders, 0);
  SaveField(solution_field, output_file_name);

  return 0;
}
