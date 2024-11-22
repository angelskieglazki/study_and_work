#include <iostream>
#include <vector>


class TypeNull;

template<typename HEAD, typename TAIL>
struct TypeList
{
    typedef HEAD Head;
    typedef TAIL Tail;
};

#define TYPELIST_1(T1)                  TypeList<T1, TypeNull>
#define TYPELIST_2(T1, T2)              TypeList<T1, TYPELIST_1(T2)>
#define TYPELIST_3(T1, T2, T3)          TypeList<T1, TYPELIST_2(T2, T3)>
using MyTypeList = TYPELIST_3(int, double, std::string);


template<class OBJECT>
struct ListContainer
{
    std::vector<OBJECT*> element;
};

template<>
struct ListContainer<TypeNull>
{
    // Пустой список
};

template<class H, class T>
struct ListContainer<TypeList<H, T>>
{
    ListContainer<H> elements;
    ListContainer<T> TailElements;
};


template<class TYPE, class H, class T>
inline size_t MainCount(ListContainer<TypeList<H, T>> const& elements, TYPE* fake)
{
    if constexpr (std::is_same_v<H, TYPE>)
    {
        return elements.elements.element.size();
    }

    if constexpr (std::is_same_v<T, TypeNull>)
        return 0;
    else
        return MainCount(elements.TailElements, fake);
}

template<class TYPE, class H, class T>
inline TYPE* MainInsert(ListContainer<TypeList<H, T>>& elements, TYPE* obj)
{
    if constexpr (std::is_same_v<H, TYPE>)
    {
        elements.elements.element.push_back(obj);
        return obj;
    }

    if constexpr (std::is_same_v<T, TypeNull>)
        return nullptr;
    else
        return MainInsert(elements.TailElements, obj);
}

template<class OBJECT_TYPES>
class TypeContainer
{
public:
    TypeContainer() = default;
    ~TypeContainer() = default;

    template<class TYPE>
    size_t Count() const {
        return MainCount(elements_, (TYPE*)nullptr);
    }

    template<class TYPE>
    bool insert(TYPE *obj) {
        TYPE* t = MainInsert(elements_, obj);
        return (t != nullptr);
    }

private:
    ListContainer<OBJECT_TYPES> elements_;
};


int main()
{
    TypeContainer<MyTypeList> worldContainer;

    int* intObj = new int(42);
    int* secondIntObj = new int(1488);
    double* doubleObj = new double(3.14);
    std::string* strObj = new std::string("Hello, world!");

    worldContainer.insert(intObj);
    worldContainer.insert(secondIntObj);
    worldContainer.insert(doubleObj);
    worldContainer.insert(strObj);

    std::cout << "Count of int objects: " << worldContainer.Count<int>() << std::endl;
    std::cout << "Count of double objects: " << worldContainer.Count<double>() << std::endl;
    std::cout << "Count of std::string objects: " << worldContainer.Count<std::string>() << std::endl;

    delete intObj;
    delete secondIntObj;
    delete doubleObj;
    delete strObj;

    return 0;
}