#include "accum6.hpp"
#include <iostream>

template<typename T>
class MultPolicy {
public:
    static constexpr  T init_val() {  return 1; }
    template<typename T1, typename T2>
    static void accumulate(T1& total, T2 const& value)
    {
        total *= value;
    }
};

int main() {
    int num[] = {1,2,3,4,5};
    std::cout<< "SUM OF = " << accum<int, AccumulationTraits<int>, MultPolicy<typename AccumulationTraits<int>::AccT>>(num, num + 5) << '\n';
    return 0;
}
