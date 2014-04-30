#ifndef _LIGHTS_H
#define _LIGHTS_H

#include "debug_utils.h"
#include <Adafruit_NeoPixel.h>

#include "WorldState.h"

#define PIN_LIGHTS 5
#define SECTOR_COUNT 7

class Lights
{
  public:
    Lights(WorldState *ms, WorldState* os);
    void begin(void);
    void setSector(byte s);
    void update(void);
  private:
  	void blank(void);
    WorldState* myState;
    WorldState* otherState;
    Adafruit_NeoPixel* strip;
    byte sector;
    unsigned long currentColor;
    unsigned long red;
    unsigned long green;
    unsigned long blue;
};

#endif
