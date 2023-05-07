//
// Created by max on 20.03.23.
//

#ifndef TRAITS_ELEMENTTYPE_HPP
#define TRAITS_ELEMENTTYPE_HPP

#include <vector>
#include <list>
#include <string>

template<typename T>
struct ElementT;

template<typename T>
struct ElementT<std::vector<T>>
{
    using Type = T;
    inline static const std::string LOG = "vec";
};

template<typename T>
struct ElementT<std::list<T>>
{
    using Type = T;
    inline static const std::string LOG = "list";
};

template<typename T, std::size_t N>
struct ElementT<T[N]>
{
    using Type = T;

    inline static const std::string LOG = "ar[n]";
};

template<typename T, std::size_t N>
struct ElementT<T(&)[N]>
{
    using Type = T;
    inline static const std::string LOG = "&ar[n]";
};

template<typename T>
struct ElementT<T[]>
{
    using Type = T;
    inline static const std::string LOG = "ar[]";
};

template<typename T>
struct ElementT<T(&)[]>
{
    using Type = T;
    inline static const std::string LOG = "&ar[]";
};

#endif //TRAITS_ELEMENTTYPE_HPP
