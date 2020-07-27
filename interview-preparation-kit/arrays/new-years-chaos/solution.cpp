#include <iostream>
#include <vector>
#include <set>

using Size = unsigned int;
using Index = Size;

Size next_power_of_2(Size n) { 
    n--; 
    n |= n >> 1; 
    n |= n >> 2; 
    n |= n >> 4; 
    n |= n >> 8; 
    n |= n >> 16; 
    n++; 
    return n; 
}  // next_power_of_2(.)

class MySet {
  // MultiSet-like structure implemented using vector-based binary tree.
  // In logarithmic time can insert a value and return number of elements,
  // which are already in the set and are less than the inserted value.
  public:
  MySet(const Size max_node_value) {
    capacity_ = next_power_of_2(max_node_value) + 1;
    t_.resize(capacity_, 0);
    root_node_val_ = capacity_ / 2;
  }  // ctr

  Size InsertAndGetNumberOfLesser(const Index value) {
    const Index a = value - 1;
    Index i{1};
    Index node_val{root_node_val_};
    Size result{0};
    Size step{root_node_val_};
    while (true) {
      ++t_[i];
      step = (step + 1) / 2;
      if (a == node_val) {
        i = 2 * i;
        break;
      } else if (a > node_val) {
        result += t_[i] - 1;
        i = 2 * i + 1;
        result -= t_[i];
        node_val = node_val + step;
      } else {
        i = 2 * i;
        node_val = node_val - step;
      }
    }  // while loop

    result += (i < capacity_) ? t_[i] : 0;
    return result;
  } // InsertAndGetNumberOfLesser(.)

private:
  Size capacity_;
  Size root_node_val_;
  std::vector<Index> t_;
};  // class MySet

int minimumBribesDirect(const std::vector<int>& q) {
  // O(n^2) solution
  const int n = q.size();
  int bribes_num{0};
  for (int i = 0; i < n-1; ++i) {
    int bribes_given{0};  // bribes given by the person with label q[i]
    const int& qi = q[i];

    // how many elements less than q[i] are to right from q[i]
    for (int j = i + 1; j < n; ++j)
      if (qi > q[j])
        ++bribes_given;
    if (bribes_given > 2)
      return -1;
    bribes_num += bribes_given;
  }
  return bribes_num;
}  // minimumBribes(.)

int minimumBribesUsingStdSet(const std::vector<int>& q) {
  // O(n*log(n)?) solution
  const int n = q.size();
  int bribes_num{0};
  std::set<int> s;
  for (int i = 0; i < n-1; ++i) {
    int bribes_given{0};  // bribes given by the person with label q[i]
    const int& qi = q[i];
    
    // how many elements less than q[i] are to right from q[i]
    s.insert(qi);
    const auto it = s.lower_bound(qi);
    const int smaller_num = qi - 1;
    const int smaller_left_num = std::distance(s.begin(), it);
    const int smaller_right_num = smaller_num - smaller_left_num;
    bribes_given = smaller_right_num;

    if (bribes_given > 2)
      return -1;
    bribes_num += bribes_given;
  }
  return bribes_num;
}  // minimumBribes(.)


int minimumBribes(const std::vector<int>& q) {
  // O(n*log(n)) solution using MySet structure
  const int n = q.size();
  int bribes_num{0};
  MySet s(n);
  for (int i = 0; i < n-1; ++i) {
    int bribes_given{0};  // bribes given by the person with label q[i]
    const int& qi = q[i];

    const Size smaller_num = qi - 1;
    const Size smaller_left_num = s.InsertAndGetNumberOfLesser(qi);
    const int smaller_right_num = smaller_num - smaller_left_num;
    bribes_given = smaller_right_num;

    if (bribes_given > 2)
      return -1;
    bribes_num += bribes_given;
  }
  return bribes_num;
}  // minimumBribes(.)


int main() {
  int n, t;
  std::cin >> t;
  for (int k = 0; k < t; ++k) {
    std::cin >> n;
    std::vector<int> q(n);
    for (int i = 0; i < n; ++i) {
      std::cin >> q[i];
    }
    const int bribes_num = minimumBribes(q);
    if (bribes_num < 0) {
      std::cout << "Too chaotic\n";
    } else {
      std::cout << bribes_num << "\n";
    }
  }
  return 0;
}
