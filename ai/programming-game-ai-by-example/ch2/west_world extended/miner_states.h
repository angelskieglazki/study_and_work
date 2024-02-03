#pragma once

#include "state.h"

class Miner;

class EnterMineAndDigForNugget : public State<Miner> {
    EnterMineAndDigForNugget() = default;
public:
    EnterMineAndDigForNugget(const EnterMineAndDigForNugget&) = delete;
    EnterMineAndDigForNugget& operator=(const EnterMineAndDigForNugget&) = delete;

    static EnterMineAndDigForNugget* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
    bool onMessage(Miner*, const Telegram&) override;
};

class VisitBankAndDepositGold : public State<Miner> {
    VisitBankAndDepositGold() = default;
public:
    VisitBankAndDepositGold(const VisitBankAndDepositGold&) = delete;
    VisitBankAndDepositGold& operator=(const VisitBankAndDepositGold&) = delete;

    static VisitBankAndDepositGold* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
    bool onMessage(Miner*, const Telegram&) override;
};

class GoHomeAndSleepTilRested : public State<Miner> {
    GoHomeAndSleepTilRested() = default;
public:
    GoHomeAndSleepTilRested(const GoHomeAndSleepTilRested&) = delete;
    GoHomeAndSleepTilRested& operator=(const GoHomeAndSleepTilRested&) = delete;

    static GoHomeAndSleepTilRested* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
    bool onMessage(Miner*, const Telegram&) override;
};

class QuenchThirst : public State<Miner> {
    QuenchThirst() = default;
public:
    QuenchThirst(const QuenchThirst&) = delete;
    QuenchThirst& operator=(const QuenchThirst&) = delete;

    static QuenchThirst* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
    bool onMessage(Miner*, const Telegram&) override;
};


class EatStew : public State<Miner> {
    EatStew() = default;
public:
    EatStew(const EatStew&) = delete;
    EatStew& operator=(const EatStew&) = delete;

    static EatStew* instance();
    void enter(Miner* ) override;
    void execute(Miner* ) override;
    void exit(Miner* ) override;
    bool onMessage(Miner*, const Telegram&) override;
};