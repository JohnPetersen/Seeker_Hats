#include "Lights.h"

Lights::Lights(WorldState* ms, WorldState* os)
{
  myState = ms;
  otherState = os;
  strip = new Adafruit_NeoPixel(3, PIN_LIGHTS, NEO_GRB + NEO_KHZ800);
}

void Lights::begin(void)
{
  // init the LED strip
  //  strip.begin();
  //  strip.show(); // Initialize all pixels to 'off'
  DEBUG_PRINT("Lights Setup Complete!");
}

void Lights::setSector(byte s)
{
  // TODO update the light status based on the current states.
  sector = s;
}

