//
// Created by max on 19.03.23.
//

#ifndef TRAITS_SUMPOLICY2_HPP
#define TRAITS_SUMPOLICY2_HPP

template<typename T1, typename T2>
class SumPolicy {
public:
    static constexpr  T1 init_val() {  return 0; }
    static void accumulate(T1& total, T2 const& value)
    {
        total += value;
    }
};

#endif //TRAITS_SUMPOLICY2_HPP
