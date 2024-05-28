// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>
struct BarWithMove {
    std::string desc;
    BarWithMove(std::string desc_) : desc(desc_) { 
        std::cout << (void*)this << "@" << desc << ": ctor" << '\n'; }
    BarWithMove(const BarWithMove& other) : desc(other.desc) { std::cout  << (void*)this << "@" << desc << ": copy-ctor(" << (void*)&other << ")\n";
    }

    BarWithMove& operator=(const BarWithMove& other)
    {
        std::cout  << (void*)this << "@" << desc << ": copy assigment ctor(" << (void*)&other << ")\n";
        desc = other.desc;
        return *this;
    }

    BarWithMove(BarWithMove&& other) : desc(other.desc) {std::cout  << (void*)this << "@" << desc << ": move-ctor(" << (void*)&other << ")\n";
    }
    BarWithMove& operator=(BarWithMove&& other)
    {
        std::cout  << (void*)this << "@" << desc << ": move assigment ctor(" << (void*)&other << ")\n";
        desc = other.desc;
        return *this;
    }
    ~BarWithMove() {
        std::cout  << (void*)this << "@" << desc << ": dtor" << '\n';
    }
};
struct FooBase {
    int i = 1;
    virtual ~FooBase() = default;
};
struct Foo : FooBase {
    BarWithMove b{ "Foo inner one"};
    // ~Foo() = default;
    //  Foo(Foo&& other) = delete;
    // Foo(const Foo& other) = delete;
    //  Foo() {}
    //Foo(const Foo& other) : b(other.b) {std::cout << "foo c\n";};
     //Foo(Foo&& other) : b(std::move(other.b)) {}
    Foo& operator=(Foo&& other) = default;
    //~Foo() = default;
};
int main() {
    Foo f;
    
    std::cout << std::is_move_constructible_v<Foo> << '\n';
    Foo f1 = std::move(f);
    std::vector<Foo> v;
    // v.emplace_back(std::move(f));
    return 0;
}