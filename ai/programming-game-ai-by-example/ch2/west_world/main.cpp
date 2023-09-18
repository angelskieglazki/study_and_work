#include <iostream>
#include <chrono>
#include <thread>
#include "locations.h"
#include "miner.h"
#include "entity_names.h"
int main()
{
    using namespace std::chrono_literals;
    Miner miner(kMinerBob);

    for (int i = 0; i < 100; ++i)
    {
        miner.update();
        std::this_thread::sleep_for(10ms);
    }
    return 0;
}