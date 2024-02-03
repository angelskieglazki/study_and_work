#include <iostream>

template <typename Derived>
class EqualityComparable {
public:
    friend bool operator!=(const Derived& lhs, const Derived& rhs) {
        return !(lhs == rhs);
    }
};

class X : public EqualityComparable<X> {
private:
    int val;
public:
    X(int v) : val(v) {}
    friend bool operator==(const X& lhs, const X& rhs) {
        return lhs.val == rhs.val;
    }
};

int main() {
    X x(11);
    X y(2);
    if (x == y) {
        std::cout<<"true"<<std::endl;
    } else {
        std::cout<<"false"<<std::endl;
    }
}