#include <iostream>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include <deque>
#include <iostream>
 
using namespace std;


class scope_guard {
public:
    enum execution { always, no_exception, exception };
 
    scope_guard(scope_guard &&) = default;
    explicit scope_guard(execution policy = always) : policy(policy) {}
 
    template<class Callable>
    scope_guard(Callable && func, execution policy = exception) : policy(policy) {
        this->operator += <Callable>(std::forward<Callable>(func));
    }
 
    template<class Callable>
    scope_guard& operator += (Callable && func) try {
        handlers.emplace_front(std::forward<Callable>(func));
        return *this;
    } catch(...) {
        if(policy != no_exception) func();
        throw;
    }
 
    ~scope_guard() {
        std::cout << std::uncaught_exceptions() << std::endl;
        if(policy == always || (std::uncaught_exceptions())) {
            for(auto &f : handlers) try {
                f(); // must not throw
            } catch(...) { /* std::terminate(); ? */ }
        }
    }
 
    void dismiss() noexcept {
        handlers.clear();
    }
 
private:
    scope_guard(const scope_guard&) = delete;
    void operator = (const scope_guard&) = delete;
 
    std::deque<std::function<void()>> handlers;
    execution policy = always;
};

 
int main() {
	scope_guard a;
 
	a += [](){ cout << "World"; };
 
	scope_guard b = std::move(a);
 
	b += [](){ cout << "Hello "; };
 
	return 0;
}