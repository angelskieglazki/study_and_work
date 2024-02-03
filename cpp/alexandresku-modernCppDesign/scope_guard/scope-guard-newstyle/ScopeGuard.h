#ifndef SCOPEGUARD_H_
#define SCOPEGUARD_H_
#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>

#include <iostream>
#include <tuple>
#include <functional>
#include <utility>
#include <type_traits>

// старый код александреску на новый лад. без макросов в FAIL и  SUCCESS, только EXIT
template <typename F, typename... Ts>
class ScopeGuard
{
private:
    F f;
    std::tuple<Ts...> args;
	bool dismissed_ = false;
public:
    template <typename FwdF, typename... FwdTs>
	requires (std::is_constructible_v<FwdTs&&, Ts> && ...)
    ScopeGuard(FwdF&& func, FwdTs&&... args)
        : f(std::forward<FwdF>(func)),
          args{std::forward<FwdTs>(args)...}
    {}

	~ScopeGuard() noexcept
	{
		if (!dismissed_)
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
	void rehire() { dismissed_ = false; }
private:
	void execute()
	{
		std::apply(f, args);
	}
};


template <typename Fun, typename... Args>
inline auto MakeGuard(Fun &&func, Args &&...args)
{
	return ScopeGuard<std::decay_t<Fun>, std::remove_cv_t<std::remove_reference_t<Args>>...>(std::forward<Fun>(func), std::forward<Args>(args)...);
}

#define CONCATENATE_DIRECT(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_DIRECT(s1, s2)
#define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)

#define SCOPE_EXIT \
	auto ANONYMOUS_VARIABLE(scopeGuard) = MakeGuard



// #define ON_BLOCK_EXIT_OBJ \
// 	auto ANONYMOUS_VARIABLE(scopeGuard) = MakeObjGuard

#endif // SCOPEGUARD_H_
