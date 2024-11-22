#include <iostream>
#include <vector>

// #include "listcontainer-ptr.hpp"
// #define LIST_CONTAINER_PTR

// #include "listcontainer.hpp"
// #define LIST_CONTAINER

#include "mapcontainer.hpp"
#define MAP_CONTAINER

#ifdef MAP_CONTAINER
int main() {
    TypeUnorderedMapContainer<int, MyTypeList> worldContainer;
    
    int intObj = 42;
    int secondIntObj = 1488;
    double doubleObj = 3.14;
    std::string strObj = "Hello, world!";
    std::string strsecondObj = "Hello, Dima!";
    Foo f{};
    worldContainer.insert(1, intObj);
    worldContainer.insert(2, secondIntObj);
    worldContainer.insert(1, doubleObj);
    worldContainer.insert(1, strObj);
    worldContainer.insert(2, strsecondObj);
    worldContainer.insert(1, f);

    std::cout << "Count of int objects: " << worldContainer.Count<int>() << std::endl;
    std::cout << "Count of double objects: " << worldContainer.Count<double>() << std::endl;
    std::cout << "Count of std::string objects: " << worldContainer.Count<std::string>() << std::endl;
    std::cout << "Count of Foo objects: " << worldContainer.Count<Foo>() << std::endl;

    return 0;
}
#endif


#ifdef LIST_CONTAINER
int main() {
    TypeContainer<MyTypeList> worldContainer;
    
    int intObj = 42;
    int secondIntObj = 1488;
    double doubleObj = 3.14;
    std::string strObj = "Hello, world!";
    std::string strsecondObj = "Hello, Dima!";
    Foo f{};
    worldContainer.insert(intObj);
    worldContainer.insert(secondIntObj);
    worldContainer.insert(doubleObj);
    worldContainer.insert(strObj);
    worldContainer.insert(strsecondObj);
    worldContainer.insert(f);

    std::cout << "Count of int objects: " << worldContainer.Count<int>() << std::endl;
    std::cout << "Count of double objects: " << worldContainer.Count<double>() << std::endl;
    std::cout << "Count of std::string objects: " << worldContainer.Count<std::string>() << std::endl;
    std::cout << "Count of Foo objects: " << worldContainer.Count<Foo>() << std::endl;

    return 0;
}
#endif

#ifdef LIST_CONTAINER_PTR
int main() {
    TypeContainer<MyTypeList> worldContainer;

    int* intObj = new int(42);
    int* secondIntObj = new int(1488);
    double* doubleObj = new double(3.14);
    std::string* strObj = new std::string("Hello, world!");
    std::string* strsecondObj = new std::string("Hello, Dima!");
    Foo f{};
    worldContainer.insert(intObj);
    worldContainer.insert(secondIntObj);
    worldContainer.insert(doubleObj);
    worldContainer.insert(strObj);
    worldContainer.insert(strsecondObj);
    worldContainer.insert(&f);

    std::cout << "Count of int objects: " << worldContainer.Count<int>() << std::endl;
    std::cout << "Count of double objects: " << worldContainer.Count<double>() << std::endl;
    std::cout << "Count of std::string objects: " << worldContainer.Count<std::string>() << std::endl;
    std::cout << "Count of Foo objects: " << worldContainer.Count<Foo>() << std::endl;

    delete intObj;
    delete secondIntObj;
    delete doubleObj;
    delete strObj;

    return 0;
}
#endif