#include <thread>
#include <iostream>
#include <string>
#include <functional>

class SayHello
{
public:
  void greeting(const std::string& message) const
  {
    std::cout << message << std::endl;
  }
};


class PrintThis
{
public:
  void operator()() const
  {
    std::cout << "this=" << this << std::endl;
  }
};


void increment(int& i) {
  ++i;
}

int main() {
//  SayHello x;
//  std::thread t(&SayHello::greeting, &x, "QWERTY");

//  std::shared_ptr<SayHello> p(new SayHello);
//  std::thread t(&SayHello::greeting, p, "HELLO");

//  PrintThis x;
//  x();
//  std::thread t(std::ref(x));
//  t.join();
//  std::thread t1(x);

  int x = 42;
  std::thread t(increment, std::ref(x));
  std::cout << x << std::endl;
  t.join();
  std::cout << x << std::endl;
  return 1;
}
