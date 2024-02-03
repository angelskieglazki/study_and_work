#include <iostream>

class Unit {
public:
    Unit(double strength, double armor)
    : strength_(strength), armor_(armor) {} 
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
    using Unit::Unit;
    double attack() { return strength_ + sword_bonus_; }
    double defense() { return armor_ + plate_bonus_; }
protected:
    static constexpr double sword_bonus_ = 2;
    static constexpr double plate_bonus_ = 3;
};

class Ogre : public Unit {
public:
    using Unit::Unit;
    double attack() { return strength_ + club_penalty_; }
    double defense() { return armor_ + leather_penalty_; }
protected:
    static constexpr double club_penalty_ = -1;
    static constexpr double leather_penalty_ = -1;
};

class VeteranUnit : public Unit {
public:
    VeteranUnit(Unit& unit, double strength_bonus, double armor_bonus)
        : Unit(strength_bonus, armor_bonus), unit_(unit) {}
    double attack() { return unit_.attack() + strength_; }
    double defense() { return unit_.defense() + armor_; }
private:
    Unit& unit_;
};
int main() {
    Knight knight(10, 5);
    Ogre ogre(12, 2);
    if (knight.hit(ogre)) {
        std::cout << "knight hit ogre" << std::endl;
    }

    VeteranUnit veteran_knight(knight, 7, 9);
    VeteranUnit veteran_ogre(ogre, 1, 9);
    if (veteran_knight.hit(veteran_ogre)) {
        std::cout << "veteran_knight hit veteran_ogre" << std::endl;
    }

    VeteranUnit vveteran_ogre(veteran_ogre, 1, 9);
        if (veteran_knight.hit(vveteran_ogre)) {
        std::cout << "veteran_knight hit veteran_ogre" << std::endl;
    }
    return 0;
}