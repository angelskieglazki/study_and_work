#pragma once

#include "state.h"

class MinersWife;

extern int randInt(int x, int y);
extern double randFloat();

#define DECLARE_SINGLETON(className) \
public: \
    static className* instance() { \
        static className i; \
        return &i; \
    } \
private: \
    className(const className&) = delete; \
    className& operator=(const className&) = delete;
   

#define DECLARE_STATE_CLASS(className) \
class className : public State<MinersWife> { \
    className() = default; \
    DECLARE_SINGLETON(className) \
public: \
    virtual void enter(MinersWife* wife); \
    virtual void execute(MinersWife* wife); \
    virtual void exit(MinersWife* wife); \
};

DECLARE_STATE_CLASS(WifesGlobalState)
DECLARE_STATE_CLASS(DoHouseWork)
DECLARE_STATE_CLASS(VisitBathroom)