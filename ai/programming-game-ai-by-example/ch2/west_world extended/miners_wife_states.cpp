#include "miners_wife_states.h"
#include "miners_wife.h"
#include "locations.h"
#include "entity_names.h"

#include <iostream>

void WifesGlobalState::enter(MinersWife* wife) {}

void WifesGlobalState::execute(MinersWife* wife) {
    if (randFloat() < 0.1) {
        wife->getFsm()->changeState(VisitBathroom::instance());
    }
}

void WifesGlobalState::exit(MinersWife* wife) {}



void DoHouseWork::enter(MinersWife* wife) {}

void DoHouseWork::execute(MinersWife* wife) {
    switch (randInt(0,2)) {
        case 0 :
            std::cout << "\n" << getNameOfEntity(wife->getId()) << ": Moppin' the floor";
            break;
        case 1:
            std::cout << "\n" << getNameOfEntity(wife->getId()) << ": Washin' the dishes";
            break;
        case 2:
            std::cout << "\n" << getNameOfEntity(wife->getId()) << ": Makin' the bed";
            break;
    }
}

void DoHouseWork::exit(MinersWife* wife){}



void VisitBathroom::enter(MinersWife* wife) {
    std::cout << "\n" << getNameOfEntity(wife->getId()) << ": Walkin' to the can. Need to powda mah pretty li'lle nose"; 
}

void VisitBathroom::execute(MinersWife* wife) {
    std::cout << "\n" << getNameOfEntity(wife->getId()) << ": Ahhhhhh! Sweet relief!";

    wife->getFsm()->revertToPreviousState();
}

void VisitBathroom::exit(MinersWife* wife) {
    std::cout << "\n" << getNameOfEntity(wife->getId()) << ": Leavin' the Jon";
}