#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);
using val_t = unsigned long long;
// Complete the andProduct function below.
long long andProduct(val_t a, val_t b) {
  val_t d = b - a;
  val_t k = 1;
  val_t result = 0;
  while (k <= b) {
    val_t ma = a % (2 * k);
    if ((ma >= k) && (d < k)) result += k;
    k <<= 1;
  }
  return result;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int n_itr = 0; n_itr < n; n_itr++) {
        string ab_temp;
        getline(cin, ab_temp);

        vector<string> ab = split_string(ab_temp);

        val_t a = stoll(ab[0]);

        val_t b = stoll(ab[1]);

        val_t result = andProduct(a, b);

        fout << result << "\n";
    }

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

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

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
