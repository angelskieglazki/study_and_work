#include "message_dispatcher.h"
#include "base_game_entity.h"
#include "time/crude_timer.h"
#include "entity_manager.h"
#include "locations.h"
#include "message_types.h"
#include "entity_names.h"

#include <iostream>
using std::cout;

MessageDispatcher *MessageDispatcher::Instance()
{
    static MessageDispatcher i;
    return &i;
}

void MessageDispatcher::DispatchMessage(double delay, int sender_id, int receiver_id, int msg, void *ExtraInfo)
{
    auto *sender = EntityMgr->getEntityFromId(sender_id);
    auto *receiver = EntityMgr->getEntityFromId(receiver_id);

    if (!receiver)
    {
        cout << "Message Dispatcher: Receiver with ID = " << receiver_id << " not found\n";
        return;
    }

    Telegram telegram(0, sender_id, receiver_id, msg, ExtraInfo);

    if (delay <= 0.0f)
    {
        cout << "\nInstant telegram dispatched at time: " << Clock->getCurrentTime()
             << " by " << getNameOfEntity(sender->getId()) << " for " << getNameOfEntity(receiver->getId())
             << ". Msg is " << msgToStr(msg);

        discharge(receiver, telegram);
    }
    else
    {
        double current_time = Clock->getCurrentTime();
        telegram.dispatchTime = current_time + delay;
        priorityQ_.insert(telegram);

        cout << "\nDelayed telegram from " << getNameOfEntity(sender->getId()) << " recorded at time "
             << Clock->getCurrentTime() << " for " << getNameOfEntity(receiver->getId())
             << ". Msg is " << msgToStr(msg);
    }
}

void MessageDispatcher::DispatchDelayedMessages()
{
    double current_time = Clock->getCurrentTime();

    while (!priorityQ_.empty()
        && priorityQ_.begin()->dispatchTime < current_time
        && priorityQ_.begin()->dispatchTime > 0)
    {
        const Telegram& telegram = *priorityQ_.begin();
        auto* receiver = EntityMgr->getEntityFromId(telegram.receiver_id);

        cout << "\nQueued telegram ready for dispatch: Sent to " 
         << getNameOfEntity(receiver->getId()) << ". Msg is " << msgToStr(telegram.msg);

        discharge(receiver, telegram);

        priorityQ_.erase(priorityQ_.begin());
    }
}

void MessageDispatcher::discharge(BaseGameEntity *receiver, const Telegram &msg)
{
    if (!receiver->handleMessage(msg))
    {
        cout << "Message Dispatcher: Message not handled by " << receiver->name() << "\n";
    }
}
