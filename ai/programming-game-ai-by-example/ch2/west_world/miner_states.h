#pragma once

#include "state.h"

class Miner;

#define DECLARE_SINGLETON(className) \
public: \
    static className* instance() { \
        static className instance; \
        return &instance; \
    } \
private: \
    className(const className&) = delete; \
    className& operator=(const className&) = delete;
   

#define DECLARE_STATE_CLASS(className) \
class className : public State { \
    className() = default; \
    DECLARE_SINGLETON(className) \
public: \
    virtual void enter(Miner* miner) {} \
    virtual void execute(Miner* miner); \
    virtual void exit(Miner* miner) {} \
};

class EnterMineAndDigForNugget : public State {
    EnterMineAndDigForNugget() = default;
public:
    EnterMineAndDigForNugget(const EnterMineAndDigForNugget&) = delete;
    EnterMineAndDigForNugget& operator=(const EnterMineAndDigForNugget&) = delete;

    static EnterMineAndDigForNugget* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
};

class VisitBankAndDepositGold : public State {
    VisitBankAndDepositGold() = default;
public:
    VisitBankAndDepositGold(const VisitBankAndDepositGold&) = delete;
    VisitBankAndDepositGold& operator=(const VisitBankAndDepositGold&) = delete;

    static VisitBankAndDepositGold* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
};

class GoHomeAndSleepTilRested : public State {
    GoHomeAndSleepTilRested() = default;
public:
    GoHomeAndSleepTilRested(const GoHomeAndSleepTilRested&) = delete;
    GoHomeAndSleepTilRested& operator=(const GoHomeAndSleepTilRested&) = delete;

    static GoHomeAndSleepTilRested* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
};

class QuenchThirst : public State {
    QuenchThirst() = default;
public:
    QuenchThirst(const QuenchThirst&) = delete;
    QuenchThirst& operator=(const QuenchThirst&) = delete;

    static QuenchThirst* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
};