#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include "Janitor.h"

// /////////////////////////////////////////////
// // тестовый код
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
        // ScopeGuard removeFriend = ObjScopeGuardImpl0<UserCont, void (UserCont::*)()>::MakeObjGuard(friends_, &UserCont::pop_back);
        auto removeFriend = MakeGuard([&friends = friends_]()
                                      { friends.pop_back(); }); // это должен быть ON_BLOCK_FAIL
        pDB_->AddFriend(name_, newFriend.GetName());
        removeFriend.dismiss();
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
// тестовый код
/////////////////////////////////////////////

void Decrement(int &x)
{
    --x;
    std::cout << "Decrement! " << x << '\n';
}
void DecrementBy(int &x, int by)
{
    x = x - by;
    std::cout << "Decrementby! " << x << '\n';
}
void UseRescource(int &counter)
{
    ++counter;
    SCOPE_EXIT(Decrement, std::ref(counter));
    SCOPE_EXIT(DecrementBy, std::ref(counter), 10);
}

int main()
{
    {
        int counter0 = 0;
        {
            SCOPE_EXIT([](int &n)
                       { n += 2; },
                       std::ref(counter0));
            SCOPE_EXIT([&counter0]()
                       { counter0 += 2; });
            SCOPE_EXIT(Decrement, std::ref(counter0));
            auto j = MakeJanitor([&counter0]() { counter0 += 2; });
            j.dismiss();
        }

        std::cout << "0counter is " << counter0 << '\n';
    }
    {
        int counter = 0;
        UseRescource(counter);
        std::cout << counter << '\n';
    }

    {

        UserDatabase db;
        User max("Max", &db);
        User slava("Slava", &db);
        User dima("Dima", &db);
        User qwerty("qwerty", &db);

        AddFriend(max, slava);
        AddFriend(max, dima);
        AddFriend(max, qwerty);

        max.printFriends();
    }

    return 0;
}
