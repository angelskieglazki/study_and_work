#include "miner_states.h"
#include "state.h"
#include "miner.h"
#include "locations.h"
#include "entity_names.h"

#include <iostream>

EnterMineAndDigForNugget *EnterMineAndDigForNugget::instance() {
  static EnterMineAndDigForNugget i;
  return &i;
}

void EnterMineAndDigForNugget::enter(Miner *miner) {
    if (miner->location() != kGoldmine) {
        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "walking to the goldmine";
        miner->changeLocation(kGoldmine);
    }
}

void EnterMineAndDigForNugget::execute(Miner *miner) {
    miner->addToGoldCarried(1);
    miner->incFatigue();

    std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "picking up a nugget";

    if (miner->pocketsFull()) {
        miner->getFsm()->changeState(VisitBankAndDepositGold::instance());
    }

    if (miner->thristy()) {
        miner->getFsm()->changeState(QuenchThirst::instance());
    }
}

void EnterMineAndDigForNugget::exit(Miner *miner) {
    std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
}


VisitBankAndDepositGold *VisitBankAndDepositGold::instance() {
  static VisitBankAndDepositGold i;

  return &i;
}

void VisitBankAndDepositGold::enter(Miner *miner) {
    if (miner->location() != kBank) {
        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "walking to the bank";
        miner->changeLocation(kBank);
    }
}

void VisitBankAndDepositGold::execute(Miner *miner) {
    miner->addToWealth(miner->goldCarried());
    miner->setGoldCarried(0);

    std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "depositing gold. Total saving now: " << miner->wealth();

    if (miner->wealth() >= ComfortLevel) {
        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "WooHoo! Rich enough for now. Back home to mah li'lle lady";

        miner->getFsm()->changeState(GoHomeAndSleepTilRested::instance());
    } else {
        miner->getFsm()->changeState(EnterMineAndDigForNugget::instance());
    }
}

void VisitBankAndDepositGold::exit(Miner *miner) {
    std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "Leavin' the bank";
}


GoHomeAndSleepTilRested *GoHomeAndSleepTilRested::instance() {
  static GoHomeAndSleepTilRested i;

  return &i;
}

void GoHomeAndSleepTilRested::enter(Miner *miner) {
    if (miner->location() != kShack) {
        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "walking home";
        miner->changeLocation(kShack);
    }
}

void GoHomeAndSleepTilRested::execute(Miner *miner) {
    if (!miner->fatigued()) {
        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "Good nap! Time to find more gold";
        miner->getFsm()->changeState(EnterMineAndDigForNugget::instance());
    } else {
        miner->decFatigue();
        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "Zzzzzzzzzzzzzzzz";
    }
}

void GoHomeAndSleepTilRested::exit(Miner *miner) {
    std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "Leavin' the shack";
} 


QuenchThirst *QuenchThirst::instance() {
  static QuenchThirst i;

  return &i;
}

void QuenchThirst::enter(Miner *miner) {
    if (miner->location() != kSaloon) {
        miner->changeLocation(kSaloon);
        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "walking to the saloon";
    }
}

void QuenchThirst::execute(Miner *miner) {
    if (miner->thristy()) {
        miner->buyAndDrinkVodka();

        std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "drinking a shot of vodka.";
        miner->getFsm()->changeState(EnterMineAndDigForNugget::instance());
    } else {
        std::cout << "\nERROR!\nERROR!\nERROR!";
    }
}

void QuenchThirst::exit(Miner *miner) {
    std::cout << "\n" << getNameOfEntity(miner->getId()) << ": " << "Leavin' the saloon";
}
