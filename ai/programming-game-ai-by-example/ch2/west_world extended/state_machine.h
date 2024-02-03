#pragma once

#include <cassert>
#include <cstring>

#include "state.h"

template<typename EntityType>
class StateMachine {
public:
    explicit StateMachine(EntityType* owner)
        : owner_(owner)
        {}

    virtual ~StateMachine() = default;

    void setCurrentState(State<EntityType>* state) { currentState_ = state; }
    void setPreviousState(State<EntityType>* state) { prevState_ = state; }
    void setGlobalState(State<EntityType>* state) { globalState_ = state; }

    void update() const {
        if (globalState_) globalState_->execute(owner_);
        if (currentState_) currentState_->execute(owner_);
    }

    bool handleMessage(const Telegram& msg) const {
        if (currentState_ && currentState_->onMessage(owner_, msg)) return true;
        if (globalState_ && globalState_->onMessage(owner_, msg)) return true;
        return false;
    }

    void changeState(State<EntityType>* newstate) {
        assert(newstate && "Invalid nullptr state");

        setPreviousState(currentState_);
        currentState_->exit(owner_);
        setCurrentState(newstate);
        currentState_->enter(owner_);
    }

    void revertToPreviousState() {
        changeState(prevState_);
    }

    bool isInState(State<EntityType>* state) const {
        return typeid(state) == typeid(*currentState_);
    }

    State<EntityType>* currentState() const { return currentState_; }
    State<EntityType>* previousState() const { return prevState_; }
    State<EntityType>* globalState() const { return globalState_; }

private:
    EntityType* owner_{nullptr};
    State<EntityType>* currentState_{nullptr};
    State<EntityType>* prevState_{nullptr};
    State<EntityType>* globalState_{nullptr};
};