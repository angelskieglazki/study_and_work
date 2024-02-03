#include <iostream>

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

class Ogre : public Unit {
public:
    Ogre(double strength, double armor) : Unit(strength, armor) {}
    double attack() { return strength_ + club_penalty_; }
    double defense() { return armor_ + leather_penalty_; }
protected:
    static constexpr double club_penalty_ = -1;
    static constexpr double leather_penalty_ = -1;
};

template<typename T>
class VeteranUnit : public T {
public:
    VeteranUnit(T&& unit, double strength_bonus, double armor_bonus)
        : T(unit), strength_bonus_(strength_bonus), armor_bonus_(armor_bonus)
        {}
    double attack() { return T::attack() + strength_bonus_; }
    double defense() { return T::defense() + armor_bonus_; }
private:
    double strength_bonus_;
    double armor_bonus_;
};
int main() {
    Knight knight(10, 5);
    Ogre ogre(12, 2);
    std::cout << "knight hit ogre " << knight.hit(ogre) << std::endl;

    VeteranUnit<Knight> vk(std::move(knight), 7, 2);
    VeteranUnit<Ogre> vo(std::move(ogre), 1, 9);
    std::cout << "veteran_knight hit veteran_ogre " << vk.hit(vo) << std::endl;

    VeteranUnit<VeteranUnit<Ogre>> vvo(std::move(vo), 1, 9);
    std::cout << "veteran_knight hit Vveteran_ogre " << vk.hit(vvo) << std::endl;
    vk.charge();
    std::cout << "veteran_knight hit Vveteran_ogre " << vk.hit(vvo) << std::endl;
    return 0;
}