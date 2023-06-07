#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <typeinfo>

template <typename T>
class MyAlloc
{
public:
    using value_type = T;

    MyAlloc() noexcept
    {
        std::cout << "MyAlloc()" << std::endl;
    }

    template <typename U>
    MyAlloc(const MyAlloc<U> &) noexcept
    {
        std::cout << "MyAlloc(const MyAlloc<U> &)" << std::endl;
    }

    T *allocate(std::size_t n)
    {
        std::cout << "allocate " << n << " of " << typeid(T).name() << std::endl;
        return static_cast<T *>(::operator new(n * sizeof(T)));
    }

    void deallocate(T *p, std::size_t n)
    {
        std::cout << "deallocate " << n << " of " << typeid(T).name() << std::endl;
        ::operator delete(p);
    }

    template <typename U, typename... Args>
    void construct(U *p, Args &&...args)
    {
        std::cout << "construct " << typeid(U).name() << std::endl;
        ::new ((void *)p) U(std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U *p)
    {
        std::cout << "destroy " << typeid(U).name() << std::endl;
        p->~U();
    }
};

template <typename T1, typename T2>
bool operator==(const MyAlloc<T1> &, const MyAlloc<T2> &)
{
    return true;
}

template <typename T1, typename T2>
bool operator!=(const MyAlloc<T1> &, const MyAlloc<T2> &)
{
    return false;
}

int main(int, char **)
{
    std::cout << "Hello, world!\n";
    // a vector with special allocator
    std::vector<int, MyAlloc<int>> v;
    v.emplace_back(1);

    // an int/float map with special allocator
    std::map<int, float, std::less<int>,
             MyAlloc<std::pair<const int, float>>>
        m;
    m.emplace(1, 1.0f);

    // a string with special allocator
    std::basic_string<char, std::char_traits<char>, MyAlloc<char>> s;
    s += "hello";

    // special string type that uses special allocator
    typedef std::basic_string<char, std::char_traits<char>,
                              MyAlloc<char>>
        MyString;

    // special string/string map type that uses special allocator
    typedef std::map<MyString, MyString, std::less<MyString>,
                     MyAlloc<std::pair<const MyString, MyString>>>
        MyMap;
    // create object of this type
    MyMap mymap;
    mymap.emplace(MyString("hello"), MyString("world"));
}
