#include <iostream>
#include <vector>

template<typename... Types>
struct TypeList {};

using MyTypeList = TypeList<int, double, std::string>;

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
    std::vector<OBJECT*> element;
};

template <typename Head, typename... Tail>
struct ListContainer<TypeList<Head, Tail...>> {
    ListContainer<Head> elements;
    ListContainer<TypeList<Tail...>> TailElements;
};

template <typename TYPE, typename TypeList>
inline size_t MainCount(const ListContainer<TypeList>& elements, TYPE* fake) {
    if constexpr (std::is_same_v<typename Front<TypeList>::Type, TYPE>) {
        return elements.elements.element.size();
    }

    return MainCount(elements.TailElements, fake);
}

template<typename TYPE>
inline size_t MainCount(const ListContainer<TypeList<>>& elements, TYPE* fake) {
    return 0;
}

template <typename TYPE, typename TypeList>
inline TYPE* MainInsert(ListContainer<TypeList>& elements, TYPE* obj) {
    if constexpr (std::is_same_v<typename Front<TypeList>::Type, TYPE>) {
        elements.elements.element.push_back(obj);
        return obj;
    }

    return MainInsert(elements.TailElements, obj);
}

template<typename TYPE>
inline TYPE* MainInsert(ListContainer<TypeList<>>& elements, TYPE* obj) {
    return nullptr;
}


template <typename OBJECT_TYPES>
class TypeContainer {
public:
    TypeContainer() = default;
    ~TypeContainer() = default;

    template <typename TYPE>
    size_t Count() const {
        return MainCount(elements_, (TYPE*)nullptr);
    }

    template <typename TYPE>
    bool insert(TYPE *obj) {
        TYPE* t = MainInsert(elements_, obj);
        return (t != nullptr);
    }

private:
    ListContainer<OBJECT_TYPES> elements_;
};

int main() {
    TypeContainer<MyTypeList> worldContainer;

    int* intObj = new int(42);
    int* secondIntObj = new int(1488);
    double* doubleObj = new double(3.14);
    std::string* strObj = new std::string("Hello, world!");
    std::string* strsecondObj = new std::string("Hello, Dima!");

    worldContainer.insert(intObj);
    worldContainer.insert(secondIntObj);
    worldContainer.insert(doubleObj);
    worldContainer.insert(strObj);
    worldContainer.insert(strsecondObj);

    std::cout << "Count of int objects: " << worldContainer.Count<int>() << std::endl;
    std::cout << "Count of double objects: " << worldContainer.Count<double>() << std::endl;
    std::cout << "Count of std::string objects: " << worldContainer.Count<std::string>() << std::endl;

    delete intObj;
    delete secondIntObj;
    delete doubleObj;
    delete strObj;

    return 0;
}
