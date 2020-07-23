#include <bits/stdc++.h>

using namespace std;

int main() {
  int c[37];  // cache
  ofstream fout(getenv("OUTPUT_PATH"));

  int s;
  cin >> s;
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  c[0] = 1;
  c[1] = 1;
  c[2] = 2;
  for (int i = 3; i < 37; ++i) c[i] = c[i - 1] + c[i - 2] + c[i - 3];
  for (int s_itr = 0; s_itr < s; s_itr++) {
    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    int res = c[n] % 10000000007;
    fout << res << "\n";
  }
  fout.close();
  return 0;
}
