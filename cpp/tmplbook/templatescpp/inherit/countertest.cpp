//
// Created by max on 08.04.23.
//

#include "objectcounter.hpp"
#include <iostream>

template<typename CharT>
class MyString : public ObjectCounter<MyString<CharT>> {
};

int main() {
    MyString<char> s1, s2;
    MyString<wchar_t> s3;

    std::cout << "Количество MyString<char>: " << MyString<char>::live() << '\n';
    std::cout << "Количество MyString<wchar_t>: " << MyString<wchar_t>::live() << '\n';
}