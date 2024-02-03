#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>

// старый код александреску, но на новый лад. класс для функций.лямбд и класс для объектов

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

class ScopeGuardImplBase
{
public:
    ScopeGuardImplBase()
    {
        std::cout << "c-tor ScopeGuardImplBase" << '\n';
    }

    ScopeGuardImplBase &operator=(const ScopeGuardImplBase &) = delete;

    void Dismiss() const noexcept { dismissed_ = true; }

protected:
    ~ScopeGuardImplBase() noexcept
    {
        std::cout << "d-tor ScopeGuardImplBase" << '\n';
    }
    ScopeGuardImplBase(const ScopeGuardImplBase &other) noexcept
        : dismissed_(other.dismissed_)
    {
        other.Dismiss();
    }

    template <typename J>
    static void SafeExecute(J &j) noexcept
    {
        if (!j.dismissed_)
        {
            try
            {
                j.Execute();
            }
            catch (...)
            {
            }
        }
    }

    mutable bool dismissed_ = false;
};

template <typename... Args>
class scope_guard : public ScopeGuardImplBase
{
public:
    static scope_guard<Args...> MakeGuard(std::function<void(Args...)> fun, Args&& ...args)
    {
        return scope_guard<Args...>(fun, std::forward<Args>(args)...);
    }
    scope_guard(std::function<void(Args...)> func, Args &&...args) : f_(func), args_(std::forward<Args>(args)...) {}
    ~scope_guard() noexcept
    {
        std::cout << "d-tor ScopeGuardImpl1" << '\n';
        SafeExecute(*this);
    }

    void dismiss() { dismissed_ = true; }

    void Execute()
    {
        func(args_);
    }
protected:

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
    std::tuple<Args...> args_;
    // int exception_count = std::uncaught_exceptions();
};


// обертка для метода MemFun у обьекта Obj, без параметров
template <typename Obj, typename MemFun, typename... Args>
class scope_guard_obj : public ScopeGuardImplBase
{
public:
    static scope_guard_obj<Obj, MemFun> MakeObjGuard(Obj &obj, MemFun memFun, Args &&...args)
    {
        return scope_guard_obj<Obj, MemFun>(obj, memFun, std::forward<Args>(args)...);
    }
    ~scope_guard_obj() noexcept
    {
        std::cout << "d-tor scope_guard_obj" << '\n';
        SafeExecute(*this);
    }

    void Execute()
    {
        std::cout << "Execute UNDO" << '\n';
        (obj_.*memFun_)();
    }

protected:
    scope_guard_obj(Obj &obj, MemFun memFun, Args &&...args) : obj_(obj), memFun_(memFun), args_(std::forward<Args>(args)...)
    {
        std::cout << "c-tor scope_guard_obj" << '\n';
    }

    Obj &obj_;
    MemFun memFun_;
    std::tuple<Args...> args_;
};

using ScopeGuard = const ScopeGuardImplBase &;

// тестовый код

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
        ScopeGuard removeFriend = scope_guard_obj<UserCont, void (UserCont::*)()>::MakeObjGuard(friends_, &UserCont::pop_back);
        pDB_->AddFriend(name_, newFriend.GetName());
        removeFriend.Dismiss();
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

void AddFriend(User& max, User& newFriend)
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


void Decrement(int& x) { --x; }
void UseRescource(int& counter)
{
    ++counter;
    ScopeGuard decrement = scope_guard<int&>::MakeGuard(Decrement, counter);
    decrement.Dismiss();
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