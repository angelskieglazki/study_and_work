#include <iostream>
#include <memory>
#include <cassert>

using std::cout;
using std::endl;

class Unit {
public:
    Unit(double strength, double armor)
    : strength_(strength), armor_(armor) {} 
    Unit(const Unit&) = default;
    virtual ~Unit() = default;

    virtual bool hit(Unit& target) { return attack() > target.defense();}
    virtual double attack() = 0;
    virtual double defense() = 0;
protected:
    double strength_;
    double armor_;
};
using Unit_ptr = std::unique_ptr<Unit>;

class Knight : public Unit {
public:
    Knight(double strength, double armor) : Unit(strength, armor), charge_bonus_(0) {}
    double attack() { auto res = strength_ + sword_bonus_ + charge_bonus_; charge_bonus_ = 0; return res; }
    double defense() { return armor_ + plate_bonus_; }
    void charge() { charge_bonus_ = 1; }
protected:
    double charge_bonus_;
    static constexpr double sword_bonus_ = 2;
    static constexpr double plate_bonus_ = 3;
};
using Knight_ptr = std::unique_ptr<Knight>;

class Ogre : public Unit {
public:
    Ogre(double strength, double armor) : Unit(strength, armor) {}
    double attack() { return strength_ + club_penalty_; }
    double defense() { return armor_ + leather_penalty_; }
protected:
    static constexpr double club_penalty_ = -1;
    static constexpr double leather_penalty_ = -1;
};

class Troll : public Unit {
    public:
    using Unit::Unit;
    double attack() { return strength_ + mace_bonus_; }
    double defense() { return armor_ + hide_bonus_; }
    protected:
    static constexpr double mace_bonus_ = 3;
    static constexpr double hide_bonus_ = 8;
};

template<typename TO, typename FROM>
static std::unique_ptr<TO> move_cast(std::unique_ptr<FROM>& p) {
    return std::unique_ptr<TO>(static_cast<TO*>(p.release()));
}

template <typename U> class VeteranUnit : public U {
    public:
    template <typename P>
    VeteranUnit(P&& p, double strength_bonus, double armor_bonus)
        : U(std::move(*move_cast<U>(p)))
        , strength_bonus_(strength_bonus)
        , armor_bonus_(armor_bonus) {}
    double attack() { return U::attack() + strength_bonus_; }
    double defense() { return U::defense() + armor_bonus_; }
    private:
    double strength_bonus_;
    double armor_bonus_;
};

template<typename T>
class DebugDecorator : public T {
public:
    using T::T;
    template <typename P>
    DebugDecorator(P&& p) : P(std::move(*move_cast<P>(p))) {}
    double attack() { auto res = T::attack(); cout << "attack: " << res << endl; return res; }
    double defense() { auto res = T::defense(); cout << "defense: " << res << endl; return res; }
};

template<typename Callable>
class CallableDecorator {
public:
    CallableDecorator( const Callable& c, const char* s) : c_(c), s_(s) {}

    template<typename ...Args>
    auto operator()(Args&& ...args) const {
        cout << "Calling " << s_ << endl;
        auto res = c_(std::forward<Args>(args) ...);
        cout << "Result: " << res << endl;
        return res;
    }
private:
    const Callable& c_;
    const std::string s_;
};

struct test_fuctor {
    int operator()(int i) const {
        return i*2;
    }
};

int foo(int i) {
    return i*2;
}

template<typename Callable>
auto decorate_debug(const Callable& c, const char* s) {
    return CallableDecorator<Callable>(c, s);
}

int main() {
    auto cd = decorate_debug(foo, "foo");
    cd(10);
    test_fuctor tf;
    auto cd1 = decorate_debug(tf, "tf");
    cd1(20);
    auto cd2 = decorate_debug([](int i) { return i*2; }, "lambda");
    cd2(30);

    Knight_ptr k(new DebugDecorator<Knight>(10, 5));
    Unit_ptr o(new DebugDecorator<Ogre>(12, 2));
    k->charge();
    cout << "Knight hits Ogre: " << k->hit(*o) << endl;
    Unit_ptr t(new Troll(14, 10));
    cout << "Knight hits Troll: " << k->hit(*t) << endl;
    Knight_ptr vk(new DebugDecorator<VeteranUnit<Knight>>(k, 7, 2));
    cout << "Veteran Knight hits Troll: " << vk->hit(*t) << endl;
    Unit_ptr vo(new DebugDecorator<VeteranUnit<Ogre>>(o, 1, 9));
    cout << "Veteran Knight hits Veteran Ogre: " << vk->hit(*vo) << endl;
    Unit_ptr vvo(new DebugDecorator<VeteranUnit<VeteranUnit<Ogre>>>(vo, 1, 9));
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
    vk->charge();
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
    return 0;
}