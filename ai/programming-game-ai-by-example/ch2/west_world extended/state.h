#pragma once

struct Telegram;
template<typename EntityType>
class State {
public:
    virtual ~State() = default;

    virtual void enter(EntityType* ) = 0;
    virtual void execute(EntityType* ) = 0;
    virtual void exit(EntityType* ) = 0;
    virtual bool onMessage(EntityType*, const Telegram&) = 0;
};