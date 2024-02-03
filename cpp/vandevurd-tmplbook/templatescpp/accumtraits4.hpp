//
// Created by max on 15.03.23.
//

#ifndef TRAITS_ACCUMTRAITS4_HPP
#define TRAITS_ACCUMTRAITS4_HPP

template<typename T>
struct AccumulationTraits;

template<>
struct AccumulationTraits<char>
{
    using AccT = int;
    static constexpr  AccT zero() {
        return 0;
    }
};

template<>
struct AccumulationTraits<short>
{
    using AccT = int;
    static constexpr  AccT zero() {
        return 0;
    }
};

template<>
struct AccumulationTraits<int>
{
    using AccT = long;
    static constexpr  AccT zero() {
        return 0;
    }
};

template<>
struct AccumulationTraits<unsigned int>
{
    using AccT = unsigned long;
    static constexpr  AccT zero() {
        return 0;
    }
};

template<>
struct AccumulationTraits<float>
{
    using AccT = double;
    static constexpr  AccT zero() {
        return 0;
    }
};
#endif //TRAITS_ACCUMTRAITS4_HPP
