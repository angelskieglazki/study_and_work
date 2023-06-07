#include <thread>
#include <stdexcept>

class scoped_thread
{
private:
  std::thread t;
public:
  scoped_thread(std::thread t_) : t(std::move(t_)) {
    if (!t.joinable()) {
      throw std::logic_error("thread is not joinable");
    }
  }
  ~scoped_thread() {
    t.join();
  }

  scoped_thread(const scoped_thread &) = delete;
  scoped_thread &operator=(const scoped_thread &) = delete;
};

void do_something(int &i) {
  ++i;
}

struct func
{
  int &i;

  func(int &i_) : i(i_) {}

  void operator()()
  {
    for (unsigned j = 0; j < 1'000'000; ++j)
    {
      do_something(i);
    }
  }
};

void do_something_in_current_thread() {}

void f() {
  int some_local_state = 0;
  scoped_thread t{std::thread(func(some_local_state))};
  do_something_in_current_thread();
}

int main() {
  f();
  return 0;
}