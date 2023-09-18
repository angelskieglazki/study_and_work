#include <iostream>
#include <chrono>
#include <thread>
#include "../../common/misc/utils.h"
#include "locations.h"
#include "miner.h"
#include "miners_wife.h"
#include "entity_names.h"
int main()
{
    using namespace std::chrono_literals;
    Miner bob(kMinerBob);
    MinersWife elsa(kElsa);

    for (int i = 0; i < 20; ++i)
    {
        bob.update();
        elsa.update();
        std::this_thread::sleep_for(300ms);
    }
    return 0;
}