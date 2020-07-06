#include <bits/stdc++.h>

using namespace std;

string ltrim(const string &);
string rtrim(const string &);
vector<string> split(const string &);
static constexpr int kMaxQueries = 100'000;
array<int, kMaxQueries> fr_count;
enum Commands { kInsert = 1, kRemove = 2, kCheckFrequency = 3 };

vector<int> freqQuery(vector<vector<int>> queries) {
  map<int, int> freq;
  vector<int> output;
  int fr;
  for (const auto &query : queries) {
    const int &command = query[0];
    const int &value = query[1];
    switch (command) {
      case kInsert:
        fr = freq[value];
        if (fr_count[fr] > 0) --fr_count[fr];
        freq[value] = ++fr;
        ++fr_count[fr];
        break;
      case kRemove:
        fr = freq[value];
        if (fr > 0) {
          if (fr_count[fr] > 0) --fr_count[fr];
          freq[value] = --fr;
          if (fr > 0) ++fr_count[fr];
        }
        break;
      case kCheckFrequency:
        int result =
            (value >= 0 && value < kMaxQueries) ? (fr_count[value] > 0) : 0;
        output.push_back(result);
        break;
    }  // switch()
  }    // loop over queries
  return output;
}  // freqQuery

int main() {
  ofstream fout(getenv("OUTPUT_PATH"));

  string q_temp;
  getline(cin, q_temp);

  int q = stoi(ltrim(rtrim(q_temp)));

  vector<vector<int>> queries(q);

  for (int i = 0; i < q; i++) {
    queries[i].resize(2);

    string queries_row_temp_temp;
    getline(cin, queries_row_temp_temp);

    vector<string> queries_row_temp = split(rtrim(queries_row_temp_temp));

    for (int j = 0; j < 2; j++) {
      int queries_row_item = stoi(queries_row_temp[j]);

      queries[i][j] = queries_row_item;
    }
  }

  vector<int> ans = freqQuery(queries);

  for (int i = 0; i < ans.size(); i++) {
    fout << ans[i];

    if (i != ans.size() - 1) {
      fout << "\n";
    }
  }

  fout << "\n";

  fout.close();

  return 0;
}

string ltrim(const string &str) {
  string s(str);

  s.erase(s.begin(),
          find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));

  return s;
}

string rtrim(const string &str) {
  string s(str);

  s.erase(
      find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
      s.end());

  return s;
}

vector<string> split(const string &str) {
  vector<string> tokens;

  string::size_type start = 0;
  string::size_type end = 0;

  while ((end = str.find(" ", start)) != string::npos) {
    tokens.push_back(str.substr(start, end - start));
    start = end + 1;
  }

  tokens.push_back(str.substr(start));

  return tokens;
}
