#pragma once

#include <iostream>
#include <math.h>

struct Telegram
{
    int sender_id;
    int receiver_id;
    int msg;
    double dispatchTime;
    void *extraInfo;

    Telegram()
        : dispatchTime(-1), sender_id(-1), receiver_id(-1), msg(-1) {}

    Telegram(double dispatchTime, int sender_id, int receiver_id, int msg, void *extraInfo = nullptr)
        : dispatchTime(dispatchTime), sender_id(sender_id), receiver_id(receiver_id), msg(msg), extraInfo(extraInfo) {}
};

const double smallestDelay = 0.25;

inline bool operator==(const Telegram &t1, const Telegram &t2)
{
    return (std::abs(t1.dispatchTime - t2.dispatchTime) < smallestDelay)
        && (t1.sender_id == t2.sender_id)
        && (t1.receiver_id == t2.receiver_id)
        && (t1.msg == t2.msg);
}

inline bool operator<(const Telegram &t1, const Telegram &t2)
{
    if (t1 == t2)
    {
        return false;
    }
    else
    {
        return (t1.dispatchTime < t2.dispatchTime);
    }
}

inline std::ostream &operator<<(std::ostream &os, const Telegram &t)
{
    os << "time: " << t.dispatchTime << "  sender: " << t.sender_id
       << "   receiver: " << t.receiver_id << "   msg: " << t.msg;

    return os;
}

template <class T>
inline T DereferenceToType(void* p)
{
  return *(T*)(p);
}