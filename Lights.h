#ifndef _LIGHTS_H
#define _LIGHTS_H

#include "debug_utils.h"
#include <Adafruit_NeoPixel.h>

#include "WorldState.h"

#define PIN_LIGHTS 11
#define SECTOR_COUNT 5

#define SCAN_DELAY_MS 300

#define CONNECTION_TIMEOUT_MS 6000

class Lights
{
  public:
    Lights(WorldState *ms, WorldState* os);
    void begin(void);
    void setSector(byte s);
    void update(void);
  private:
  	void setPixel(short n, unsigned long color);
    WorldState* myState;
    WorldState* otherState;
    Adafruit_NeoPixel* strip;
    short sector;
    bool sectorUpdated;
    unsigned long currentColor;
    unsigned long red;
    unsigned long green;
    unsigned long blue;
    long nextUpdateTime;
};

#endif
