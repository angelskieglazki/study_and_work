//
// Created by max on 20.03.23.
//

#include "elementtype.hpp"
#include <vector>
#include <iostream>

template<typename T>
void printElementType(T const& c)
{
    std::cout << "Container of "
                << typeid(typename ElementT<T>::Type).name()
                << " elements "
                << ElementT<T>::LOG
                << '\n';
}

template<typename T>
void printElementType1(T& c)
{
    std::cout << "Container of "
              << typeid(typename ElementT<T>::Type).name()
              << " elements "
              << ElementT<T>::LOG
              << '\n';
}

int main() {
    std::vector<bool> b;
    std::list<int> l;
    int arr[42];
    auto& a = arr;

    printElementType(b);
    printElementType(l);
    printElementType1(arr);
    printElementType1(a);
    return 0;
}
