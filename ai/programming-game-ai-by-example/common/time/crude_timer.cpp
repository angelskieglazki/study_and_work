#include "crude_timer.h"

CrudeTimer *CrudeTimer::instance()
{
    static CrudeTimer i;

    return &i;
}