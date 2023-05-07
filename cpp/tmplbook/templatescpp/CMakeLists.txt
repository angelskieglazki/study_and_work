cmake_minimum_required(VERSION 3.14)
project(templatescpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(accum6 accum6.cpp accumtraits4.hpp sumpolicy1.hpp accum6.hpp)
add_executable(accum7 accum7.cpp sumpolicy2.hpp accum7.hpp)
add_executable(elementtype elementtype.cpp elementtype.hpp elementtype.cpp elementtype.hpp)
add_executable(countertest inherit/countertest.cpp inherit/objectcounter.hpp)
add_executable(equalitycomparable inherit/equalitycomparable.cpp)