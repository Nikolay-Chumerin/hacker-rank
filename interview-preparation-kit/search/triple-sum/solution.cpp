#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);

void uniquify_and_sort(vector<int>& a) {
  unordered_set<int> s;
  for (int i : a) s.insert(i);
  a.assign(s.begin(), s.end());
  sort(a.begin(), a.end());
}  // uniquify_and_sort(.)

// Complete the triplets function below.
long triplets(vector<int>& a, vector<int>& b, vector<int>& c) {
  uniquify_and_sort(a);
  uniquify_and_sort(b);
  uniquify_and_sort(c);
  long a_idx{0};
  long b_idx{0};
  long c_idx{0};
  long n{0};
  for (b_idx = 0; b_idx < b.size(); ++b_idx) {
    int& b_val = b[b_idx];
    while (a_idx < a.size() && a[a_idx] <= b_val) ++a_idx;
    while (c_idx < c.size() && c[c_idx] <= b_val) ++c_idx;
    n += a_idx * c_idx;
  }  // loop over b
  return n;
}  // triplets(...)

int main() {
  char default_output_file_name[] = "output.txt";
  char* output_file_name = getenv("OUTPUT_PATH");
  if (!output_file_name) output_file_name = default_output_file_name;
  ofstream fout(output_file_name);

  string lenaLenbLenc_temp;

  getline(cin, lenaLenbLenc_temp);

  vector<string> lenaLenbLenc = split_string(lenaLenbLenc_temp);
  int lena = stoi(lenaLenbLenc[0]);
  int lenb = stoi(lenaLenbLenc[1]);
  int lenc = stoi(lenaLenbLenc[2]);

  string arra_temp_temp;
  getline(cin, arra_temp_temp);
  vector<string> arra_temp = split_string(arra_temp_temp);
  vector<int> arra(lena);
  for (int i = 0; i < lena; i++) {
    arra[i] = stoi(arra_temp[i]);
  }

  string arrb_temp_temp;
  getline(cin, arrb_temp_temp);
  vector<string> arrb_temp = split_string(arrb_temp_temp);
  vector<int> arrb(lenb);
  for (int i = 0; i < lenb; i++) {
    arrb[i] = stoi(arrb_temp[i]);
  }

  string arrc_temp_temp;
  getline(cin, arrc_temp_temp);
  vector<string> arrc_temp = split_string(arrc_temp_temp);
  vector<int> arrc(lenc);
  for (int i = 0; i < lenc; i++) {
    arrc[i] = stoi(arrc_temp[i]);
  }

  long ans = triplets(arra, arrb, arrc);
  fout << ans << "\n";
  fout.close();
  return 0;
}

vector<string> split_string(string input_string) {
  string::iterator new_end =
      unique(input_string.begin(), input_string.end(),
             [](const char& x, const char& y) { return x == y and x == ' '; });

  input_string.erase(new_end, input_string.end());

  while (input_string[input_string.length() - 1] == ' ') {
    input_string.pop_back();
  }

  vector<string> splits;
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
}
