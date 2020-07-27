#include <bits/stdc++.h>
using namespace std;

long maximumSumUsingPrefixSums(const vector<long> &a, const long m) {
  long max_sum = 0;

  const long a_sz = a.size();
  vector<long> p(a_sz + 1);
  p[0] = 0;
  for (long i = 0; i < a_sz; ++i) {
    p[i + 1] = p[i] + a[i] % m;
  }

  for (long len = a_sz; len > 0; --len) {
    for (long i = 0; i < a_sz - len + 1; ++i) {
      max_sum = max(max_sum, (p[i + len] - p[i]) % m);
    }  // loop over start indices
    if (max_sum == m - 1) return m - 1;
  }  // loop over lengths
  return max_sum;
}  // maximumSumUsingPrefixSums(..)

long maximumSum(const vector<long> &a, const long m) {
  long max_sum = 0;
  long sum = 0;
  for (const long &x : a) {
    const long y = x; //  % m;
    sum = max(y % m, (sum + y) % m);
    max_sum = max(max_sum, sum);
  }
  return max_sum;
}  // maximumSum(..)

vector<string> split_string(string input_string) {
  string::iterator new_end =
      unique(input_string.begin(), input_string.end(),
             [](const char &x, const char &y) { return x == y and x == ' '; });

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

int main() {
  char default_output_file_name[] = "output.txt";
  char *output_file_name = getenv("OUTPUT_PATH");
  if (!output_file_name) output_file_name = default_output_file_name;
  ofstream fout(output_file_name);

  int q;
  cin >> q;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  for (int q_itr = 0; q_itr < q; q_itr++) {
    string nm_temp;
    getline(cin, nm_temp);
    vector<string> nm = split_string(nm_temp);
    int n = stoi(nm[0]);
    long m = stol(nm[1]);

    string a_temp_temp;
    getline(cin, a_temp_temp);
    vector<string> a_temp = split_string(a_temp_temp);
    vector<long> a(n);

    for (int i = 0; i < n; i++) {
      long a_item = stol(a_temp[i]);
      a[i] = a_item;
    }
    long result = maximumSum(a, m);
    fout << result << "\n";
  }

  fout.close();

  return 0;
}  // main()
