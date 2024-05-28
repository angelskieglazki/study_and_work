#include <iostream>

#define GENERATE_HAS_MEMBER_TRAIT(member)                                      \
                                                                               \
  template <typename T> struct has_member_##member {                           \
  private:                                                                     \
    struct fallback {                                                          \
      int member;                                                              \
    };                                                                         \
    struct derived : T, fallback {};                                           \
                                                                               \
    using true_t = char[1];                                                    \
    using false_t = char[2];                                                   \
    template <typename U>                                                      \
    static constexpr auto _trait(decltype(U::member) *) -> false_t &;          \
    template <typename U> static constexpr auto _trait(U *) -> true_t &;       \
                                                                               \
  public:                                                                      \
    static constexpr bool value =                                              \
        sizeof(_trait<derived>(nullptr)) == sizeof(true_t);                    \
  };                                                                           \
                                                                               \
  template <typename T>                                                        \
  constexpr bool has_member_##member##_v = has_member_##member<T>::value;

struct Foo1 {
  const int Tra2323ceId1 = 1;
  int bar() { return Tra2323ceId1; }
};

struct Bar {
  using Foo = struct Foo1;
  int TraceId = 1;
  int baz() { return TraceId; }
  Foo f;
  Foo *fptr = nullptr;
};

GENERATE_HAS_MEMBER_TRAIT(bar)
GENERATE_HAS_MEMBER_TRAIT(Tra2323ceId1)

int main() {
  const Foo1 test;
  std::cout << has_member_bar_v<decltype(test)> << std::endl;
  std::cout << has_member_Tra2323ceId1_v<Foo1> << std::endl;
  std::cout << has_member_Tra2323ceId1_v<Bar> << std::endl;

  return 0;
}