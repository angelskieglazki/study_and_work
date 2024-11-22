#include <iostream>
#include <unordered_map>
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

// template <typename, typename>
// struct ContainerUnorderedMap;

template<class KEY_TYPE, class OBJECT>
struct ContainerUnorderedMap
{
    std::unordered_map<KEY_TYPE, OBJECT> element;
};

template <typename KEY_TYPE>
struct ContainerUnorderedMap<KEY_TYPE, TypeList<>>
{
};

template <typename KEY_TYPE, typename Head, typename... Tail>
struct ContainerUnorderedMap<KEY_TYPE, TypeList<Head, Tail...>> {
    ContainerUnorderedMap<KEY_TYPE, Head> elements;
    ContainerUnorderedMap<KEY_TYPE, TypeList<Tail...>> TailElements;
};

template <typename KEY_TYPE, typename TYPE, typename TypeList>
inline size_t MainCount(const ContainerUnorderedMap<KEY_TYPE, TypeList>& elements, TYPE) {
    if constexpr (std::is_same_v<typename Front<TypeList>::Type, TYPE>) {
        return elements.elements.element.size();
    }
    return MainCount(elements.TailElements, TYPE{});
}

template<typename KEY_TYPE, typename TYPE>
inline size_t MainCount(const ContainerUnorderedMap<KEY_TYPE, TypeList<>>& elements, TYPE) {
    return 0;
}

template <typename KEY_TYPE, typename TYPE, typename TypeList>
inline void MainInsert(ContainerUnorderedMap<KEY_TYPE, TypeList>& elements, const KEY_TYPE& k, TYPE obj) {
    if constexpr (std::is_same_v<typename Front<TypeList>::Type, TYPE>) {
        elements.elements.element.emplace(k, obj);
    } else {
        MainInsert(elements.TailElements, k, obj);
    }
}

template<typename KEY_TYPE, typename TYPE>
inline void MainInsert(ContainerUnorderedMap<KEY_TYPE, TypeList<>>& elements, const KEY_TYPE& k, TYPE obj) {
    // No-op for an empty list
}

template <typename KEY_TYPE, typename OBJECT_TYPES>
class TypeUnorderedMapContainer {
public:
    TypeUnorderedMapContainer() = default;
    ~TypeUnorderedMapContainer() = default;

    template <typename TYPE>
    size_t Count() const {
        return MainCount(elements_, TYPE{});
    }

    template <typename TYPE>
    void insert(const KEY_TYPE& k, TYPE obj) {
        MainInsert(elements_, k, obj);
    }

private:
    ContainerUnorderedMap<KEY_TYPE, OBJECT_TYPES> elements_;
};

