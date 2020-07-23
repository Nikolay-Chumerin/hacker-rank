#include <bits/stdc++.h>

using namespace std;

bool IsSpecialSubstring(const string& s, const int start, const int len) {
  const int sz = s.size();
  bool result{false};
  if (start >= 0 && start < sz && len > 0 && start + len <= sz) {
    const char& c = s[start];
    result = true;
    for (int i = 0; i < len / 2; ++i) {
      if (s[start + i] != c || s[start + len - i - 1] != c) {
        result = false;
        break;
      }  //  if
    }  // for
  }  // if
  return result;
}  // IsSpecialSubstring(...)


long SubstrCountNaive(const int n, const string& s) {
  const auto sz = s.size();
  long result = sz;
  for (int len = 2; len <= sz; ++len) {
    for (int i = 0; i < sz - len + 1; ++i) {
      result += IsSpecialSubstring(s, i, len);
    }  // i-loop
  }  // loop over lengths
  return result;
}  // long SubstrCountNaive(..)

long SubstrCount(const int n, const string& s) {
  long result{0};
  char prev_prev_char{0}, prev_char{0}, curr_char{0};
  int prev_prev_count{0}, prev_count{0}, curr_count{0};
  for (const char& c : s) {
    if (c != curr_char) {
      result += curr_count * (curr_count + 1) / 2;  //  1 + 2 + ... + n  =  (1 + n) * n  / 2
      if (prev_count == 1 && prev_prev_char == curr_char) {
        result += min(prev_prev_count, curr_count);
      }
      prev_prev_char = prev_char;
      prev_prev_count = prev_count;
      prev_char = curr_char;
      prev_count = curr_count;
      curr_char = c;
      curr_count = 0;
    }  // if (c != curr_char)
    ++curr_count;
  }  // loop over chars in s
  result += curr_count * (curr_count + 1) / 2;  //  1 + 2 + ... + n  =  (1 + n) * n  / 2
  if (prev_count == 1 && prev_prev_char == curr_char) {
    result += min(prev_prev_count, curr_count);
  }
  return result;
}  // long SubstrCount(..)

int main() {
  char default_output_file_name[] = "output.txt";
  char* output_file_name = getenv("OUTPUT_PATH");
  if (!output_file_name) output_file_name = default_output_file_name;
  ofstream fout(output_file_name);

  int n;
  cin >> n;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
  string s;
  getline(cin, s);
  long result = SubstrCount(n, s);
  fout << result << "\n";
  fout.close();
  return 0;
}
