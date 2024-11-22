#include <iostream>
#include <vector>
#include <string>
#include <type_traits>

template<typename... Types>
struct TypeList {};

struct Foo{
  int i;
};
using MyTypeList = TypeList<int, double, std::string, Foo>;

template <typename List>
struct Front;

template<typename Head, typename... Tail>
struct Front<TypeList<Head, Tail...>>
{
    using Type = Head;
};

template<typename List>
struct is_empty {
    static constexpr bool value = false;
};

template<>
struct is_empty<TypeList<>> {
    static constexpr bool value = true;
};

template <typename>
struct ListContainer;

template<class OBJECT>
struct ListContainer
{
    std::vector<OBJECT> element;
};

template <typename Head, typename... Tail>
struct ListContainer<TypeList<Head, Tail...>> {
    ListContainer<Head> elements;
    ListContainer<TypeList<Tail...>> TailElements;
};

template <typename TYPE, typename TypeList>
inline size_t MainCount(const ListContainer<TypeList>& elements, TYPE) {
    if constexpr (std::is_same_v<typename Front<TypeList>::Type, TYPE>) {
        return elements.elements.element.size();
    }
    return MainCount(elements.TailElements, TYPE{});
}

template<typename TYPE>
inline size_t MainCount(const ListContainer<TypeList<>>& elements, TYPE) {
    return 0;
}

template <typename TYPE, typename TypeList>
inline void MainInsert(ListContainer<TypeList>& elements, TYPE obj) {
    if constexpr (std::is_same_v<typename Front<TypeList>::Type, TYPE>) {
        elements.elements.element.push_back(obj);
    } else {
        MainInsert(elements.TailElements, obj);
    }
}

template<typename TYPE>
inline void MainInsert(ListContainer<TypeList<>>& elements, TYPE obj) {
    // No-op for an empty list
}

template <typename OBJECT_TYPES>
class TypeContainer {
public:
    TypeContainer() = default;
    ~TypeContainer() = default;

    template <typename TYPE>
    size_t Count() const {
        return MainCount(elements_, TYPE{});
    }

    template <typename TYPE>
    void insert(TYPE obj) {
        MainInsert(elements_, obj);
    }

private:
    ListContainer<OBJECT_TYPES> elements_;
};

