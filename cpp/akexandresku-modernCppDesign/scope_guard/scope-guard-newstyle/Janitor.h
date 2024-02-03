#ifndef JANITOR_H_
#define JANITOR_H_

#include <memory>
#include "ScopeGuard.h"

struct ICmd_
{
    virtual void dismiss() noexcept = 0;
    // virtual void rehire() noexcept = 0;
    virtual ~ICmd_() noexcept = default;
};

template <typename T>
class CmdAdaptor : public ICmd_, protected T
{
public:
    // template <typename Fun>
    // CmdAdaptor(Fun fun) : T(fun) {}

    template <typename Fun, typename... Args>
    CmdAdaptor(Fun fun, Args &&...args) : T(fun, std::forward<Args>(args)...) {}

    void dismiss() noexcept override
    {
        std::cout << "CMDADAPTOR!!!!!\n";
        T::dismiss();
    }
};

class Janitor
{
public:
    Janitor() noexcept = default;

    template <typename F, typename... Args>
    Janitor(F pFun, Args &&...args) : spCmd_(
                                          std::make_unique<CmdAdaptor<ScopeGuard<F, Args...>>>(pFun, std::forward<Args>(args)...))
    {
    }

    Janitor(const Janitor &other) noexcept = default;

    Janitor &operator=(const Janitor &other) noexcept
    {
        if (spCmd_)
            spCmd_->dismiss();
        spCmd_ = std::move(other.spCmd_);
        return *this;
    }

    void dismiss() noexcept
    {
        if (spCmd_)
            spCmd_->dismiss();
    }

protected:
    mutable std::unique_ptr<ICmd_> spCmd_;
};


template <typename Fun, typename... Args>
inline auto MakeJanitor(Fun &&func, Args &&...args)
{
	return Janitor(std::forward<Fun>(func), std::forward<Args>(args)...);
}

#endif // JANITOR_H_
