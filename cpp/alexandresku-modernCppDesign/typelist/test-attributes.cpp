#include <iostream>
#include <map>
#include <string>
#include <type_traits>

// Уникальный идентификатор
using UnnyId = int32_t;


// Шаблонный класс для работы с атрибутами
template <typename T>
using Attrs = std::map<UnnyId, T>;

// Специальная структура для void-атрибутов
struct void_attr {};

// Основной класс AttrContainer, который принимает TypeList
template <typename... AttrTypes>
class AttrContainer {
public:
    // Методы для добавления атрибутов
    template <typename T>
    void addAttr(UnnyId id, const T& value) {
        getAttrContainer<T>()[id] = value;
    }

    // Методы для получения атрибутов
    template <typename T>
    T getAttr(UnnyId id) const {
        const auto& container = getAttrContainer<T>();
        auto it = container.find(id);
        if (it != container.end()) {
            return it->second;
        }
        throw std::runtime_error("Attribute not found");
    }

    // Методы для удаления атрибутов
    template <typename T>
    void removeAttr(UnnyId id) {
        getAttrContainer<T>().erase(id);
    }

    // Метод для проверки наличия атрибута
    template <typename T>
    bool hasAttr(UnnyId id) const {
        return getAttrContainer<T>().count(id) > 0;
    }

private:
    // Вспомогательный метод для получения нужного контейнера атрибутов
    template <typename T>
    Attrs<T>& getAttrContainer() {
        return std::get<Attrs<T>>(containers_);
    }

    // Константный вариант метода для получения контейнера атрибутов
    template <typename T>
    const Attrs<T>& getAttrContainer() const {
        return std::get<Attrs<T>>(containers_);
    }

    // Кортеж, содержащий все контейнеры для каждого типа из TypeList
    std::tuple<Attrs<AttrTypes>...> containers_;
};

// Пример использования
int main() {
    // Создаем контейнер с набором типов
    AttrContainer<bool, int32_t, float, std::string, void_attr> attrs;

    // Пример добавления атрибутов
    attrs.addAttr<bool>(1, true);
    attrs.addAttr<int32_t>(2, 123);
    attrs.addAttr<float>(3, 45.67f);
    attrs.addAttr<std::string>(4, "example");
    attrs.addAttr<void_attr>(5, void_attr{});

    // Пример получения атрибутов
    std::cout << "bool attr: " << attrs.getAttr<bool>(1) << "\n";
    std::cout << "int32_t attr: " << attrs.getAttr<int32_t>(2) << "\n";
    std::cout << "float attr: " << attrs.getAttr<float>(3) << "\n";
    std::cout << "string attr: " << attrs.getAttr<std::string>(4) << "\n";

    // Проверка наличия атрибута
    if (attrs.hasAttr<void_attr>(5)) {
        std::cout << "void_attr with id 5 exists.\n";
    }

    // Удаление атрибута
    attrs.removeAttr<int32_t>(2);

    return 0;
}
