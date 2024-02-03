#include "miners_wife.h"

void MinersWife::update() {
    stateMachine_->update();
}

bool MinersWife::handleMessage(const Telegram &msg)
{
    return stateMachine_->handleMessage(msg);
}
