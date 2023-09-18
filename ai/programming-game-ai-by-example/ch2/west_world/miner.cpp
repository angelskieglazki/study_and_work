#include "miner.h"
#include "miner_states.h"

Miner::Miner(int id)
    : BaseGameEntity(id),
    location_(kShack),
    goldCarried_(0),
    moneyInBank_(0),
    thirst_(0),
    fatigue_(0),
    currentState_(GoHomeAndSleepTilRested::instance()) {
        
    }

void Miner::update() {
    thirst_++;

    if (currentState_) {
        currentState_->execute(this);
    }
}


void Miner::changeState(State* new_state) {
    assert(currentState_ && new_state);

    currentState_->exit(this);
    currentState_ = new_state;
    currentState_->enter(this);
}


void Miner::addToGoldCarried(int val) {
    goldCarried_ += val;
    if (goldCarried_ < 0) {
        goldCarried_ = 0;
    }
}


bool Miner::fatigued() const {
    if (fatigue_ >= TirednessThreshold) {
        return true;
    }

    return false;
}


void Miner::addToWealth(int val) {
    moneyInBank_ += val;

    if (moneyInBank_ < 0) {
        moneyInBank_ = 0;
    }
}


bool Miner::thristy() const {
    if (thirst_ >= ThirstLevel) {
        return true;
    }

    return false;
}