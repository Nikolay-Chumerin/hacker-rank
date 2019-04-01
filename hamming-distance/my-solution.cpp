#include <bits/stdc++.h>

using namespace std;
#define VERBOSED
static constexpr size_t N = 65536;
using LongCString = char[N];
LongCString both_strings[2] __attribute__((__aligned__(256)));
char* s;
char* ss;
size_t n;
//-----------------------------------------------------------------------------------
void Set(const int l, const int r, const char ch) {
#ifdef VERBOSE
  cerr << "Set(s, l=" << l << ", r=" << r << ", ch='" << ch << "): s='" << s << "' -> ";
#endif
  std::memset(s + l - 1, ch, r - l + 1);
#ifdef VERBOSE
  cerr << "'" << s << "'\n";
#endif
}  // Set(....)
//-----------------------------------------------------------------------------------
void SwapSubstrings(const int l1, const int r1, const int l2,
                    const int r2) {
#ifdef VERBOSE
  cerr <<   "SwapSubstrings(s, l1=" << l1 << ", r1=" << r1 << ", l2='" << l2 << ", r2="
      << r2 << "): s='" << s << "' -> ";
#endif
  // s = s.substr(0, l1 - 1) + s.substr(l2 - 1, r2 - l2 + 1) +
  //     s.substr(r1, l2 - 1 - r1) + s.substr(l1 - 1, r1 - l1 + 1) + s.substr(r2);

  // s.substr(0, l1 - 1)
  size_t dst_offset = 0;
  size_t src_offset = 0;
  size_t chunk_size = l1 - 1;
  memcpy(ss + dst_offset, s + src_offset, chunk_size);

  // s.substr(l2 - 1, r2 - l2 + 1)
  dst_offset += chunk_size;
  src_offset = l2 - 1;
  chunk_size = r2 - l2 + 1;
  memcpy(ss + dst_offset, s + src_offset, chunk_size);

  // s.substr(r1, l2 - 1 - r1)
  dst_offset += chunk_size;
  src_offset = r1;
  chunk_size = l2 - 1 - r1;
  memcpy(ss + dst_offset, s + src_offset, chunk_size);

  // s.substr(l1 - 1, r1 - l1 + 1)
  dst_offset += chunk_size;
  src_offset = l1 - 1;
  chunk_size = r1 - l1 + 1;
  memcpy(ss + dst_offset, s + src_offset, chunk_size);

  // s.substr(r2)
  dst_offset += chunk_size;
  src_offset = r2;
  chunk_size = n - r2;
  memcpy(ss + dst_offset, s + src_offset, chunk_size);

  swap(ss, s);

#ifdef VERBOSE
  cerr << "'" << s << "'\n";
#endif
}  // SwapSubstrings(....)
//-----------------------------------------------------------------------------------
void FlipSubstring(const int l, const int r) {
#ifdef VERBOSE
  cerr << "FlipSubstring(s, l=" << l << ", r=" << r << "): s='" << s << "' -> ";
#endif
  char* pl = &s[l - 1];
  char* pr = &s[r - 1];
  for (int i = 0; i <= (r - l) / 2; ++i) {
    const char cl = *pl;
    const char cr = *pr;
    if (cl != cr) {
      *pl = cr;
      *pr = cl;
    }
    ++pl;
    --pr;
  }
#ifdef VERBOSE
  cerr << "'" << s << "'\n";
#endif
}  // FlipSubstring(...)
//-----------------------------------------------------------------------------------
void PrintSubstring(const int l, const int r) {
#ifdef VERBOSE
  cerr << "PrintSubstring(s, l=" << l << ", r=" << r << ") -> ";
#endif
  char* substring = &s[l-1];
  char t = s[r];
  s[r] = '\0';
  puts(substring);
#ifdef VERBOSE
  cerr << "'" << substring << "'\n";
#endif
  s[r] = t;
}  // PrintSubstring(...)
//-----------------------------------------------------------------------------------
void PrintHammingDistance(const int l1, const int l2, const int len) {
#ifdef VERBOSE
  cerr << "PrintHammingDistance(s, l1=" << l1 << ", l2=" << l2 << ", len=" << len << ") -> ";
#endif
  int dd = 0;

  uint64_t *pu1 = reinterpret_cast<uint64_t *>(&s[l1 - 1]);
  uint64_t *pu2 = reinterpret_cast<uint64_t *>(&s[l2 - 1]);
  for (int i = len/8; i; --i) {
     dd += __builtin_popcountll((*pu1++) ^ (*pu2++));
  }

  int d = 0;
  const size_t remaining_part_offset = (len >> 3) << 3;
  char *p1 = &s[l1 - 1 + remaining_part_offset];
  char *p2 = &s[l2 - 1 + remaining_part_offset];
  for (int i = len % 8; i; --i) {
    d += (*p1++) ^ (*p2++);
  }
  cout << dd / 2 + d / 3 << "\n";
#ifdef VERBOSE
  cerr << d << "\n";
#endif
}  // PrintHammingDistance(...)
//-----------------------------------------------------------------------------------
int main() {
  s = both_strings[0];
  ss = both_strings[1];
  cin >> n;
  cin >> s;
  int m;
  cin >> m;
  char command, ch;
  int l, r, ll, rr;
  for (int i = 0; i < m; ++i) {
#ifdef VERBOSE
    cerr << "command: " << command << ":";
#endif

    cin >> command >> l >> r;
    switch (command) {
    case 'C':
      cin >> ch;
      Set(l, r, ch);
      break;
    case 'S':
      cin >> ll >> rr;
      SwapSubstrings(l, r, ll, rr);
      break;
    case 'R':
      FlipSubstring(l, r);
      break;
    case 'W':
      PrintSubstring(l, r);
      break;
    case 'H':
      cin >> ll;
      PrintHammingDistance(l, r, ll);
      break;
    }  // switch (command)
  }  // main loop
  return 0;
}
