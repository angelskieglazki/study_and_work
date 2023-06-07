#include <iostream>
#include <string>

template<typename T>
inline void PRINT_ELEMENTS(const T& coll, const std::string& optstr="") {
  std::cout << optstr;
  for (const auto& e : coll) {
    std::cout << e << " ";
  }
  std::cout << std::endl;
}