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

template<typename T>
class VeteranUnit : public T {
public:
    template<typename U>
    VeteranUnit(U&& u, double strength_bonus, double armor_bonus)
        : T(std::move(*move_cast<T>(u))), strength_bonus_(strength_bonus), armor_bonus_(armor_bonus)
        {}
    double attack() { return T::attack() + strength_bonus_; }
    double defense() { return T::defense() + armor_bonus_; }
private:
    double strength_bonus_;
    double armor_bonus_;
};
int main() {
    Knight_ptr k(new Knight(10, 5));
    Unit_ptr o(new Ogre(12, 2));
    k->charge();
    cout << "k hit o " << k->hit(*o) << endl;

    Unit_ptr t(new Troll(14, 10));
    cout << "k hit t " << k->hit(*t) << endl;

    Knight_ptr vk(new VeteranUnit<Knight>(k, 7, 2));
    cout << "vk hit t " << vk->hit(*t) << endl;

    Unit_ptr vo(new VeteranUnit<Ogre>(o, 1, 9));
    cout << "Veteran Knight hits Veteran Ogre: " << vk->hit(*vo) << endl;

    Unit_ptr vvo(new VeteranUnit<VeteranUnit<Ogre>>(vo, 1, 9));
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
    vk->charge();
    cout << "Veteran Knight hits Veteran2 Ogre: " << vk->hit(*vvo) << endl;
    {
        Unit_ptr k_(new Knight(10, 5));
        Unit_ptr vk_(new VeteranUnit<Ogre>(k_, 7, 2));
    }

    return 0;
}