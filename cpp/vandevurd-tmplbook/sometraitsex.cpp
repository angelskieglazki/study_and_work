#include <iostream>
#include <string>
#include <type_traits>

// Трейт, определяющий, является ли объект движимым
template <typename T>
struct is_movable {
    static constexpr bool value = std::is_floating_point<typename T::position_type>::value;
};

// Трейт, определяющий, имеет ли объект метод атаки
template <typename T>
struct has_attack {
    template <typename U>
    static auto test(U* obj) -> decltype(obj->attack(), std::true_type{});

    template <typename>
    static std::false_type test(...);

    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

// Базовый класс для всех объектов игры
class GameObject {
public:
    GameObject(const std::string& name) : name_(name) {}

    std::string getName() const {
        return name_;
    }

private:
    std::string name_;
};

// Класс игрока
class Player : public GameObject {
public:
    using position_type = float;

    Player(const std::string& name, float posX, float posY)
        : GameObject(name), posX_(posX), posY_(posY) {}

    float getPositionX() const {
        return posX_;
    }

    float getPositionY() const {
        return posY_;
    }

private:
    float posX_;
    float posY_;
};

// Класс врага
class Enemy : public GameObject {
public:
    using position_type = int;

    Enemy(const std::string& name, int posX, int posY)
        : GameObject(name), posX_(posX), posY_(posY) {}

    int getPositionX() const {
        return posX_;
    }

    int getPositionY() const {
        return posY_;
    }

    void attack() {
        std::cout << "Enemy attacks!" << std::endl;
    }

private:
    int posX_;
    int posY_;
};

int main() {
    Player player("John", 10.5f, 20.0f);
    Enemy enemy("Dragon", 100, 200);

    // Используем трейты для проверки свойств объектов и выполнения соответствующих действий

    // Проверяем, является ли объект игрока движимым
    if (is_movable<Player>::value) {
        std::cout << player.getName() << " is movable." << std::endl;
    } else {
        std::cout << player.getName() << " is not movable." << std::endl;
    }

    // Проверяем, является ли объект игрока атакующим
    if (has_attack<Player>::value) {
        std::cout << player.getName() << " can attack." << std::endl;
    } else {
        std::cout << player.getName() << " cannot attack." << std::endl;
    }

    // Проверяем, является ли объект врага движимым
    if (is_movable<Enemy>::value) {
        std::cout << enemy.getName() << " is movable." << std::endl;
    } else {
        std::cout << enemy.getName() << " is not movable." << std::endl;
    }

    // Проверяем, является ли объект врага атакующим
    if (has_attack<Enemy>::value) {
        std::cout << enemy.getName() << " can attack." << std::endl;
        enemy.attack();
    } else {
        std::cout << enemy.getName() << " cannot attack." << std::endl;
    }

    return 0;
}















