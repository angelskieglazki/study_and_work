#include <thread>
#include <iostream>
#include <string>

class SayHello
{
public:
  void operator()() const
  {
    std::cout << "hello" << std::endl;
  }
};


class Greeting
{
  std::string message;
  public:
    explicit Greeting(const std::string& message) : message(message)
    {
    }

    void operator()() const
    {
      std::cout << message << std::endl;
    }
};


void greeting(const std::string& message, unsigned i) {
  std::cout << message << i <<std::endl;
}

int main() {
//  std::thread t{SayHello()};
//  std::thread t(Greeting("Goodbye"));
//  std::thread t(std::bind(greeting, "hi!", 200000000));
  std::thread t(greeting, "hi!", 200000000);
  t.join();
  return 1;
}
