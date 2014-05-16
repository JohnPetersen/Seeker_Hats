#ifndef _COMPASS_H
#define _COMPASS_H

#include "debug_utils.h"
#include <Adafruit_LSM303.h>
#include "WorldState.h"

class Compass
{
  public:
    Compass(WorldState* ms, float minMagCal[], float maxMagCal[]);
    void begin(void);
    void update(void);
  private:
    WorldState* myState;
    Adafruit_LSM303* compass;
    Adafruit_LSM303::lsm303MagData calMin;
    Adafruit_LSM303::lsm303MagData calMax;
    float getHeading(void);
};

#endif
