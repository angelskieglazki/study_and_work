#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>

//1) этот код под новый стиль
//2) цепочка колбеков
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

// функция с одни параметром, можно расширить для другого количества параметров, путём
// создания новых классв ScopeGuardImpl2 3 4 и так далее. В старом с++ нет вариадиков...
template <typename F, typename P1>
class ScopeGuardImpl1 : public ScopeGuardImplBase
{
public:
    static ScopeGuardImpl1<F, P1> MakeGuard(F fun, P1 parm)
    {
        return ScopeGuardImpl1<F, P1>(fun, parm);
    }
    ~ScopeGuardImpl1() noexcept
    {
        std::cout << "d-tor ScopeGuardImpl1" << '\n';
        SafeExecute(*this);
    }

    void Execute()
    {
        fun_(p1_);
    }

protected:
    ScopeGuardImpl1(F f, P1 p1) : fun_(f), p1_(p1)
    {
        std::cout << "c-tor ScopeGuardImpl1" << '\n';
    }

    F fun_;
    const P1 p1_;
};

// обертка для метода MemFun у обьекта Obj, без параметров
template <typename Obj, typename MemFun>
class ObjScopeGuardImpl0 : public ScopeGuardImplBase
{
public:
    static ObjScopeGuardImpl0<Obj, MemFun> MakeObjGuard(Obj &obj, MemFun memFun)
    {
        return ObjScopeGuardImpl0<Obj, MemFun>(obj, memFun);
    }
    ~ObjScopeGuardImpl0() noexcept
    {
        std::cout << "d-tor ObjScopeGuardImpl0" << '\n';
        SafeExecute(*this);
    }

    void Execute()
    {
        std::cout << "Execute UNDO" << '\n';
        (obj_.*memFun_)();
    }

protected:
    ObjScopeGuardImpl0(Obj &obj, MemFun memFun) : obj_(obj), memFun_(memFun)
    {
        std::cout << "c-tor ObjScopeGuardImpl0" << '\n';
    }

    Obj &obj_;
    MemFun memFun_;
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
        ScopeGuard removeFriend = ObjScopeGuardImpl0<UserCont, void (UserCont::*)()>::MakeObjGuard(friends_, &UserCont::pop_back);
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
    ScopeGuard decrement = ScopeGuardImpl1<void (*)(int&), int&>::MakeGuard(Decrement, counter);
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