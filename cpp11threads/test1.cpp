#include <thread>
#include <iostream>

void my_thread_func() {
  std::cout << "Hello thread!" << std::endl;
}

int main()
{
  std::thread t(my_thread_func);
  t.join();
  return 1;
}
