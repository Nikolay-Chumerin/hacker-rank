#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

static constexpr size_t kModulo = 2147483648;  // 1 << 31;
static constexpr size_t kMask = kModulo - 1;
static constexpr size_t kMaxSize = kModulo / 8;
// std::bitset<kModulo> bs;

int main() {
  size_t n, s, p, q;
  // std::cin >> n >> s >> p >> q;
  n = 100;
  // s = 1972857628U;
  // p = 499841161;
  // q = 437173653;
  s = 569099406UL;
  p = 1607140150UL;
  q = 823906344UL;
  size_t idx = s & kMask;

  // bs[idx] = 1;

  for (int i = 1; i < n; ++i) {
    idx = (idx * p + q) & kMask;
    std::cerr << idx << "\n";
    // bs[idx] = 1;
  }
  // std::cout << bs.count() << "\n";
  return 0;
}
