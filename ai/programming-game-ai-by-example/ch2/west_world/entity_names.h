#pragma once

#include <string>

enum {
    kMinerBob,
    kElsa
};

inline  std::string getNameOfEntity(int n) {
    switch (n) {
        case kMinerBob: return "Miner Bob";
        case kElsa: return "Elsa";
        default: return "Unknown";
    }
}