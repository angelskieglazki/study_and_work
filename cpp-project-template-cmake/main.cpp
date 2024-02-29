#include <iostream>
class FirstInterface {
public:
  virtual void displayFirst() const = 0;
  virtual ~FirstInterface() = default;
};

class SecondInterface {
public:
  virtual void displaySecond() const = 0;
  virtual ~SecondInterface() = default;
};

class ThirdInterface : public FirstInterface, public SecondInterface {
public:
  virtual ~ThirdInterface() = default;
  virtual void displayThird() const = 0;
};

class BaseClass : public ThirdInterface {
private:
  int someData;

public:
  BaseClass(int data) : someData(data) {}

  void displaySecond() const override {
    std::cout << "Displaying from SecondInterface " << getSomeData()
              << std::endl;
  }
  void displayThird() const override {
    std::cout << "Displaying from ThirdInterface" << std::endl;
  }

  int getSomeData() const { return someData; }
};

class Foo_impl : public BaseClass {
public:
  Foo_impl(int data, int data_impl) : BaseClass(data), foo_impl(data_impl) {}
  void displayFirst() const override {
    std::cout << "Displaying from FirstInterface " << foo_impl << std::endl;
  }

private:
  int foo_impl;
};
class Bar_impl : public BaseClass {
public:
  Bar_impl(int data, char *data_impl) : BaseClass(data), bar_impl(data_impl) {}
  void displayFirst() const override {
    std::cout << "Displaying from FirstInterface " << bar_impl << std::endl;
  }

private:
  std::string bar_impl;
};

int main() {
  ThirdInterface *test = new Foo_impl{1, 4};
  test->displayFirst();
  test->displaySecond();

  delete test;
  ThirdInterface *test1 = new Bar_impl{1, "zalupka"};
  test1->displayFirst();
  test1->displaySecond();

  delete test1;
  return 0;
}






class sendInterface{};
class TickInterface{};
class SerializeInterface{};

class worldInterface : public sendInterface, TickInterface, SerializeInterface {
  void fn1();
};

class world : public worldInterface {
  void fn1();
  operator sendInterface& ();
  TickInterface& getTicker();
  SerializeInterface& getSerializeer();
};





///
h:
class sendInterface{};
class TickInterface{};
class SerializeInterface{};

class worldInterface : public virtual sendInterface, TickInterface, SerializeInterface {
  void fn1();
};

cpp:
class sender: public sendInterface {};
class Ticker: public TickInterface{};
class Serializeer: public SerializeInterface{};

class world : public worldInterface, sender, Ticker, Serializeer {
  void fn1();
};

