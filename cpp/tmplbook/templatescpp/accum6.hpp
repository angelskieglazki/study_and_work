//
// Created by max on 15.03.23.
//

#ifndef TRAITS_ACCUM6_HPP
#define TRAITS_ACCUM6_HPP

#include "accumtraits4.hpp"
#include "sumpolicy1.hpp"

template<typename T,
        typename Traits = AccumulationTraits<T>,
        typename Policy = SumPolicy<typename Traits::AccT>>
auto accum(T const* beg, T const* end) {
    using AccT = typename Traits::AccT;
    AccT total = Policy::init_val();

    while (beg != end) {
        Policy::accumulate(total, *beg);
        ++beg;
    }

    return total;
}

#endif //TRAITS_ACCUM6_HPP
