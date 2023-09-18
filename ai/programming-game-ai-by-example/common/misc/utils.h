#pragma once

#include <random>

double randFloat() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    return dis(gen);
}

int randInt(int x, int y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(x, y);

    return dis(gen);
}