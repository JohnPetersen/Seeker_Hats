#ifndef _GPS_WRAPPER_H
#define _GPS_WRAPPER_H

#include <debug_utils.h>
#include <Adafruit_GPS.h>

#include "WorldState.h"

#define PIN_GPS_TX 8
#define PIN_GPS_RX 7

#define MAX_LONG 2147483647L

class GPS_Wrapper
{
  public:
    GPS_Wrapper(WorldState *ws);
    void begin(void);
    void processGps(void);
    void updatePosition(void);
  private:
    WorldState* myState;
    Adafruit_GPS* GPS;
    long toBAMS(float v, char d);
};

#endif
