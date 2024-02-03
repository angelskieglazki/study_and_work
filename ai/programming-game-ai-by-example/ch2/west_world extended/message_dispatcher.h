#pragma once

#include "../../common/messagibg/telegram.h"

class BaseGameEntity;

const double SEND_MSG_IMMEDIATELY = 0.0f;
const int NO_ADDITIONAL_INFO = 0;

#define Dispatch MessageDispatcher::Instance()

class MessageDispatcher
{
  MessageDispatcher() {}
  MessageDispatcher(const MessageDispatcher &) = delete;
  MessageDispatcher &operator=(const MessageDispatcher &) = delete;

  void discharge(BaseGameEntity *receiver, const Telegram &msg);

public:
  static MessageDispatcher *Instance();

  void DispatchMessage(double  delay,
                       int    sender_id,
                       int    receiver_id,
                       int    msg,
                       void*  ExtraInfo);

  void DispatchDelayedMessages();

private:
  std::set<Telegram> priorityQ_;
};
