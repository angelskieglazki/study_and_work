//
// Created by max on 15.03.23.
//

#ifndef TRAITS_SUMPOLICY1_HPP
#define TRAITS_SUMPOLICY1_HPP

template<typename T>
class SumPolicy {
public:
    static constexpr  T init_val() {  return 0; }
    template<typename T1, typename T2>
    static void accumulate(T1& total, T2 const& value)
    {
        total += value;
    }
};
#endif //TRAITS_SUMPOLICY1_HPP
