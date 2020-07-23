#include <cmath>
#include <iostream>
using namespace std;

int fibonacci(int n) {
  static const double sqrt5 = sqrt(5);
  return int(0.5 + pow((1.0 + sqrt5) / 2.0, n) / sqrt5);
}

int main() {
  int n;
  cin >> n;
  cout << fibonacci(n);
  return 0;
}
