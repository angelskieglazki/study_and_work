#pragma once

#include <cassert>
#include <cstring>

#include "state.h"

template<typename EntityType>
class StateMachine {
public:
    StateMachine(EntityType* owner)
        : owner_(owner)
        , currentState_(nullptr)
        , prevState_(nullptr)
        , globalState_(nullptr)
        {}

    virtual ~StateMachine() = default;

    void setCurrentState(State<EntityType>* state) { currentState_ = state; }
    void setPreviousState(State<EntityType>* state) { prevState_ = state; }
    void setGlobalState(State<EntityType>* state) { globalState_ = state; }

    void update() const {
        if (globalState_) globalState_->execute(owner_);
        if (currentState_) currentState_->execute(owner_);
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
    EntityType* owner_;
    State<EntityType>* currentState_;
    State<EntityType>* prevState_;
    State<EntityType>* globalState_;
};