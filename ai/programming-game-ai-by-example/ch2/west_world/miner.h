#pragma once

#include <string>
#include <cassert>

#include "base_game_entity.h"
#include "locations.h"

class State;

const int ComfortLevel = 5;
const int MaxNuggets = 3;
const int ThirstLevel = 5;
const int TirednessThreshold = 5;

class Miner : public BaseGameEntity {
public:
    explicit Miner(int id);
    void update() override;

    void changeState(State* new_state);

    ELocationType location() const { return location_; }
    void changeLocation(ELocationType new_location) { location_ = new_location; }

    int goldCarried() const { return goldCarried_; }
    void setGoldCarried(int val) { goldCarried_ = val; }
    void addToGoldCarried(int val);
    bool pocketsFull() const { return goldCarried_ >= MaxNuggets; }

    bool fatigued() const;
    void decFatigue() { --fatigue_; }
    void incFatigue() { ++fatigue_; }

    int wealth() const { return moneyInBank_; }
    void setWealth(int val) { moneyInBank_ = val; }
    void addToWealth(int val);

    bool thristy() const;
    void buyAndDrinkVodka() { thirst_ = 0; moneyInBank_ -=2; }
private:
    State* currentState_ = nullptr;
    ELocationType location_;

    int goldCarried_;
    int moneyInBank_;

    int thirst_;
    int fatigue_;
};