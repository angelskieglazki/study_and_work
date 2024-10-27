#include <iostream>
#include <string>
template <typename T, typename... Rest>
inline void hashCombine(std::size_t& seed, T const& v, Rest&&... rest) {
  seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  (int[]){0, (hashCombine(seed, std::forward<Rest>(rest)), 0)...};
}

inline void hash_combine(std::size_t& seed) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest&&... rest) {
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
    hash_combine(seed, std::forward<Rest>(rest)...);
}

struct foo {
  int i;
  std::string s;
};
#include <vector>
#include <cassert>
#include <cstring>
std::vector<uint64_t> fetchBytes(int pos, size_t size, std::vector<uint8_t> data_) {
  assert(data_.size() >= size_t(pos) + size);
  assert(size % sizeof(uint64_t) == 0);
  const uint64_t* begin = (const uint64_t*)&data_[size_t(pos)];
  std::vector<uint64_t> ret(begin, begin + (size / sizeof(uint64_t)));
  pos += size;
  return ret;
}

std::vector<uint64_t> fetch_bytes(int pos, size_t size, std::vector<uint8_t> data_) {
  assert(data_.size() >= size_t(pos) + size);
  assert(size % sizeof(uint64_t) == 0);
  //   std::vector<uint64_t> ret(size / sizeof(uint64_t));

  // // Используем memcpy для корректного копирования данных
  // std::memcpy(ret.data(), &data_[size_t(pos)], size);
  std::vector<uint64_t> ret(, begin + (size / sizeof(uint64_t)));
  pos += size;
  return ret;
}

int main() {
  
  uint16_t u0 = 123;
  uint16_t u1 = 12323;
  uint16_t u2 = 23123;
  uint16_t u3 = 3123;
  uint16_t u4 = 13223;
  uint16_t u5 = 823;


  std::size_t s1 = 0;
  std::size_t s2 = 0;

  hashCombine(s1, u0, u1, u2, u3, u4, u5);
  hash_combine(s2, u0, u1, u2, u3, u4, u5);

  if (s1 == s2) {
    std::cout << "good" << '\n';
  }

  std::vector<uint8_t> data_ = {12,2,3,54,5,67,7,8,11,22,33,44,55,46,177,88};

  auto vec1 = fetchBytes(0, 16, data_);

  for (auto i : vec1) {
    std::cout << i << ' ';
  }
  std::cout << '\n';

  auto vec2 = fetch_bytes(0, 16, data_);

  for (auto i : vec2) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
  return 0;
}
