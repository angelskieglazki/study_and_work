#include <iostream>
#include <vector>
#include <algorithm>
struct MeanValue {
  void operator()(int elem) {
    ++num;
    sum += elem;
  }

  double value() {
    return static_cast<double>(sum) / static_cast<double>(num);
  }
  private:
    int num;
    int sum;
};
int main() {
  std::vector<int> v = { 2,4,6};

  auto mv = std::for_each(std::begin(v), std::end(v), MeanValue());

  std::cout << mv.value();
}