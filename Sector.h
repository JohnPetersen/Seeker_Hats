#ifndef _SECTOR_H
#define _SECTOR_H

#include "debug_utils.h"
#include "WorldState.h"

#define RAD2DEG 57.2957795
#define ERROR_SECTOR 99
#define SECTOR_COUNT 5

class Sector
{
  public:
    Sector(WorldState* ms, WorldState* os);
    byte getCurrent(void);
  private:
    WorldState* myState;
    WorldState* otherState;
    byte current;
    byte candidate;
    byte calculate(void);
    byte smooth(byte s);
};

#endif
