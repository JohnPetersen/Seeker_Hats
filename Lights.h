#ifndef _LIGHTS_H
#define _LIGHTS_H

#include "debug_utils.h"
#include <Adafruit_NeoPixel.h>

#include "WorldState.h"

#define PIN_LIGHTS 5

class Lights
{
  public:
    Lights(WorldState *ms, WorldState* os);
    void begin(void);
    void setSector(byte s);
  private:
    WorldState* myState;
    WorldState* otherState;
    Adafruit_NeoPixel* strip;
    byte sector;
};

#endif
