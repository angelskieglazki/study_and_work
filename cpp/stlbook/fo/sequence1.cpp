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

    std::generate_n(std::back_inserter(coll), 9, IntSequence(1));

    PRINT_ELEMENTS(coll);

    std::generate(std::next(std::begin(coll)), coll.end(), IntSequence(42));

    PRINT_ELEMENTS(coll);
}
