#pragma once

#include <string>

enum message_type
{
    kHiHoneyImHome,
    kStewReady
};

inline std::string msgToStr(int msg)
{
    switch (msg)
    {
    case kHiHoneyImHome:
        return "Hi Honey, I'm home!";
    case kStewReady:
        return "Stew is ready!";
    default:
        return "Unknown message type";
    }
}