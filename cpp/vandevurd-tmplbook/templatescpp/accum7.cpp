#include "accum7.hpp"
#include <iostream>

int main() {
    int num[] = {1,2,3,4,5};
    std::cout<< "SUM OF = " << accum(num, num + 5) << '\n';
    return 0;
}
