//
// Created by max on 08.04.23.
//

#ifndef TEMPLATESCPP_OBJECTCOUNTER_HPP
#define TEMPLATESCPP_OBJECTCOUNTER_HPP

#include <cstddef>

template<typename CountedTpe>
class ObjectCounter
{
private:
    static inline std::size_t count = 0;

protected:
    ObjectCounter() {
        ++count;
    }

    ObjectCounter(ObjectCounter<CountedTpe> const&) {
        ++count;
    }

    ObjectCounter(ObjectCounter<CountedTpe>&&) {
        ++count;
    }

    ~ObjectCounter() {
        --count;
    }

public:
    static std::size_t live() {
        return count;
    }
    
};

#endif //TEMPLATESCPP_OBJECTCOUNTER_HPP
