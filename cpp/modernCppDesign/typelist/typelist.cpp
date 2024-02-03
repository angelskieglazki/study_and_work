#include <iostream>
#include <utility>
#include <functional>

using namespace std;

template <typename... Ts>
struct typelist
{
};

struct empty_type
{
};

namespace detail
{
    template <typename TL>
    struct length;
    template <template <typename...> typename TL,
              typename... Ts>
    struct length<TL<Ts...>>
    {
        using type =
            std::integral_constant<std::size_t, sizeof...(Ts)>;
    };
}
template <typename TL>
using length_t = typename detail::length<TL>::type;
template <typename TL>
constexpr std::size_t length_v = length_t<TL>::value;

namespace detail
{
    template <std::size_t I, std::size_t N, typename TL>
    struct at_type;

    template <std::size_t I, std::size_t N, template <typename...> typename TL, typename T, typename... Ts>
    struct at_type<I, N, TL<T, Ts...>>
    {
        using type = std::conditional_t<I == N, T, typename at_type<I, N + 1, TL<Ts...>>::type>;
    };

    template <std::size_t I, std::size_t N>
    struct at_type<I, N, typelist<>>
    {
        using type = empty_type;
    };
}

template <std::size_t I, typename TL>
using at_t = typename detail::at_type<I, 0, TL>::type;

struct game_unit
{
    int attack;
    int defense;
};

struct upgrade_defense
{
    void operator()(game_unit &u)
    {
        u.defense = static_cast<int>(u.defense * 1.2);
    }
};

struct upgrade_attack
{
    void operator()(game_unit &u)
    {
        u.attack += 2;
    }
};

template <typename TL>
struct apply_functors
{
private:
    template <size_t I>
    static void apply(game_unit &unit)
    {
        using F = at_t<I, TL>;
        std::invoke(F{}, unit);
    }

    template <size_t... I>
    static void apply_all(game_unit &unit, std::index_sequence<I...>)
    {
        (apply<I>(unit), ...);
    }

public:
    void operator()(game_unit &unit) const
    {
        apply_all(unit, std::make_index_sequence<length_v<TL>>{});
    }
};

void upgrade_unit(game_unit &unit)
{
    using upgrade_types =
        typelist<upgrade_defense, upgrade_attack>;
    apply_functors<upgrade_types>{}(unit);
}
int main()
{
    game_unit u{100, 50};
    std::cout << u.attack << " " << u.defense;

    upgrade_unit(u);
    std::cout << u.attack << " " << u.defense;

    return 0;
}