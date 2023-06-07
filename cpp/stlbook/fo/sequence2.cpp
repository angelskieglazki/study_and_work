#include <iostream>
#include <list>
#include <algorithm>
#include <iterator>
#include "../stl/print.hpp"

class IntSequence
{
private:
  int value;

public:
  IntSequence(int v) : value(v) {}
  int operator()();
};

int IntSequence::operator()()
{
  return value++;
}

int main(int, char **)
{
  std::list<int> coll;
  IntSequence seq(1);

  std::generate_n<std::back_insert_iterator<std::list<int>>,
                  int, IntSequence &>(std::back_inserter(coll), 4, seq);
  PRINT_ELEMENTS(coll);

  std::generate_n(std::back_inserter(coll), 4, IntSequence(42));
  PRINT_ELEMENTS(coll);
  
  std::generate_n(std::back_inserter(coll), 4, seq);
  PRINT_ELEMENTS(coll);

  std::generate_n(std::back_inserter(coll), 4, seq);
  PRINT_ELEMENTS(coll);
}
