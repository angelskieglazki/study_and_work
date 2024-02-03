#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>

namespace helper
{
    template <std::size_t... Ts>
    struct index
    {
    };

    template <std::size_t N, std::size_t... Ts>
    struct gen_seq : gen_seq<N - 1, N - 1, Ts...>
    {
    };

    template <std::size_t... Ts>
    struct gen_seq<0, Ts...> : index<Ts...>
    {
    };
}

template <typename... Args>
class scope_guard
{
public:
    scope_guard(std::function<void(Args...)> func, Args &&...args) : f_(func), args_(std::move<Args>(args)...) {}
    ~scope_guard() noexcept
    {
        if (exception_count != std::uncaught_exceptions())
        {
            try
            {
                execute();
            }
            catch (...)
            {
            }
        }
    }

    void dismiss() { dismissed_ = true; }

private:
    void execute()
    {
        func(args_);
    }

    template <typename... Ts>
    void func(std::tuple<Ts...> &tup)
    {
        func(tup, helper::gen_seq<sizeof...(Ts)>{});
    }

    template <typename... Ts, std::size_t... Is>
    void func(std::tuple<Ts...> &tup, helper::index<Is...>)
    {
        f_(std::get<Is>(tup)...);
    }

private:
    std::function<void(Args...)> f_;
    [[no_unique_address]] std::tuple<Args...> args_;
    bool dismissed_ = false;
    int exception_count = std::uncaught_exceptions();
};

// /////////////////////////////////////////////
// // тестовый код для ObjScopeGuardImpl0
// /////////////////////////////////////////////

class UserDatabase
{
public:
    void AddFriend(const std::string &name, const std::string &friendName)
    {
        if (friendName == "qwerty")
        {
            // это просто для теста исключение, моделируем что в базе данных произошло исключение
            throw std::runtime_error("I don't like qwerty!!!");
        }

        friendsDB_[name].push_back(friendName);
    }

private:
    using FriendList = std::unordered_map<std::string, std::vector<std::string>>;
    FriendList friendsDB_;
};

class User
{
public:
    explicit User(const std::string &name, UserDatabase *pDB) : name_(name), pDB_(pDB) {}
    std::string GetName() const { return name_; }
    void AddFriend(User &newFriend)
    {
        friends_.push_back(&newFriend);
        //ScopeGuard removeFriend = ObjScopeGuardImpl0<UserCont, void (UserCont::*)()>::MakeObjGuard(friends_, &UserCont::pop_back);
        scope_guard<> removeFriend([&friends = friends_]() { friends.pop_back(); });
        pDB_->AddFriend(name_, newFriend.GetName());
        // removeFriend.dismiss();
    }
    void printFriends() const
    {
        for (const auto &friendName : friends_)
        {
            std::cout << friendName->GetName() << '\n';
        }
    }

private:
    using UserCont = std::vector<User *>;
    UserCont friends_;
    std::string name_;
    UserDatabase *pDB_;
};

void AddFriend(User &max, User &newFriend)
{
    try
    {
        max.AddFriend(newFriend);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

/////////////////////////////////////////////
// тестовый код для ScopeGuardImpl1
/////////////////////////////////////////////

void Decrement(int& x) { --x; std::cout << "Decrement! " << x << '\n'; }
void DecrementBy(int& x, int by) { x = x - by; std::cout << "Decrement! " << x << '\n'; }
void UseRescource(int& counter)
{
    ++counter;
    scope_guard<int&> decrement(Decrement, counter);
    //scope_guard<int&, int> decrement(DecrementBy, counter, 10);
    // SCOPEGUARD(Decrement, std::ref(counter));
    // decrement.dismiss();
}

int main()
{
    {
        int counter = 0;
        UseRescource(counter);
        std::cout << counter << '\n';
    }

    UserDatabase db;
    User max("Max", &db);
    User slava("Slava", &db);
    User dima("Dima", &db);
    User qwerty("qwerty", &db);

    AddFriend(max, slava);
    AddFriend(max, dima);
    AddFriend(max, qwerty);

    max.printFriends();

    return 0;
}










// https://stackoverflow.com/questions/10270328/the-simplest-and-neatest-c11-scopeguard
// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4189.pdf
// https://stackoverflow.com/questions/16868129/how-to-store-variadic-template-arguments


