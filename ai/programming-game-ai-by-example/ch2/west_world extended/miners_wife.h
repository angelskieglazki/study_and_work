#pragma once

#include <string>

#include "state.h"
#include "base_game_entity.h"
#include "locations.h"
#include "miners_wife_states.h"
#include "miner.h"
#include "state_machine.h"


class MinersWife : public BaseGameEntity {
public:
    explicit MinersWife(int id)
        : BaseGameEntity(id)
        , stateMachine_(std::make_unique<StateMachine<MinersWife>>(this))
        , location_(kShack)
        , cooking_(false)
        {
            stateMachine_->setCurrentState(DoHouseWork::instance());
            stateMachine_->setGlobalState(WifesGlobalState::instance());
        }

    ~MinersWife() = default;
    void update() override;
    StateMachine<MinersWife>* getFsm() const { return stateMachine_.get(); }
    ELocationType location() const { return location_; }
    void changeLocation(ELocationType new_location) { location_ = new_location; }
    bool handleMessage(const Telegram& msg) override;
private:
    std::unique_ptr<StateMachine<MinersWife>> stateMachine_ = nullptr;
    ELocationType location_;
    bool cooking_;
};