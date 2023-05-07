//
// Created by max on 19.03.23.
//

#ifndef TRAITS_ACCUM7_HPP
#define TRAITS_ACCUM7_HPP

#include "accumtraits4.hpp"
#include "sumpolicy2.hpp"

template<typename T,
        template<typename, typename> class Policy = SumPolicy,
        typename Traits = AccumulationTraits<T>>
auto accum(T const* beg, T const* end)
{
    using AccT = typename Traits::AccT;
    AccT total = Policy<AccT, T>::init_val();

    while (beg != end) {
        Policy<AccT, T>::accumulate(total, *beg);
        ++beg;
    }

    return total;
}
#endif //TRAITS_ACCUM7_HPP
