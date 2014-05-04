#ifndef _LIGHTS_H
#define _LIGHTS_H

#include "debug_utils.h"
#include <Adafruit_NeoPixel.h>

#include "WorldState.h"

#define PIN_LIGHTS 5
#define SECTOR_COUNT 5

#define SCAN_DELAY_MS 250

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
    short sector;
    bool sectorUpdated;
    unsigned long currentColor;
    unsigned long red;
    unsigned long green;
    unsigned long blue;
    long scannerUpdateTime;
};

#endif
