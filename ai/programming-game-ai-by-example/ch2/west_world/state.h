#pragma once

class Miner;

class State {
public:
    virtual ~State() = default;

    virtual void enter(Miner* ) = 0;
    virtual void execute(Miner* ) = 0;
    virtual void exit(Miner* ) = 0;
};