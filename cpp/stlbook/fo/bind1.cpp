#include <functional>
#include <iostream>

int main()
{
  auto plus10 = std::bind(std::plus<int>(), std::placeholders::_1, 10);
  std::cout << plus10(5) << std::endl;

  auto plus10times2 = std::bind(std::multiplies<int>(),
                                std::bind(std::plus<int>(), std::placeholders::_1, 10), 2);
  std::cout << plus10times2(5) << std::endl;

  auto pow3 = std::bind(std::multiplies<int>(), std::bind(std::multiplies<int>(), std::placeholders::_1, std::placeholders::_1), std::placeholders::_1);
  std::cout<<pow3(4) << std::endl;

  auto inversDivide = std::bind(std::divides<double>(), std::placeholders::_2, std::placeholders::_1);
  std::cout<<inversDivide(49, 7) << std::endl;
}