#include "Lights.h"

Lights::Lights(WorldState* ms, WorldState* os)
{
  myState = ms;
  otherState = os;
  strip = new Adafruit_NeoPixel(SECTOR_COUNT, PIN_LIGHTS, NEO_GRB + NEO_KHZ800);
  currentColor = strip->Color(0,0,0);
  red = strip->Color(255,0,0);
  green = strip->Color(0,255,0);
  blue = strip->Color(0,0,255);
  scannerUpdateTime = 0;
}

void Lights::begin(void)
{
  // init the LED strip
  strip->begin();
  strip->show(); // Initialize all pixels to 'off'
  DEBUG_PRINT("Lights Setup Complete!");
}

void Lights::setSector(byte s)
{
  // TODO update the light status based on the current states.
  sector = s;
}

void Lights::update()
{
  // Choose color based on alarm states
  if (myState->isButtonFlagSet())
  {
  	if (otherState->isButtonFlagSet())
  	{
  	  // both alarms are set, switch between blue and red
  	  currentColor = currentColor == red ? blue : red;
  	}
  	else
  	{
  	  // Only my alarm is set, go blue
  	  currentColor = blue;
  	}
  }
  else
  {
  	if (otherState->isButtonFlagSet())
  	{
  	  // Only the remote alarm is set go red
  	  currentColor = red;
  	}
  	else
  	{
  	  // No alarm is set, go green
  	  currentColor = green;
  	}
  }
  // blank the current lights
  blank();
  // Choose update style based on validity of our state. Do we have fixes?
  DBPRINT("LIGHT: ");
  if (myState->isFixFlagSet() && otherState->isFixFlagSet())
  {
    // We have good fixes, display the pointer
    strip->setPixelColor(sector, currentColor);
    DBPRINT(sector);
  }
  else if (millis() > scannerUpdateTime)
  {
    // Missing one or both fixes, display a larson scanner
    if (sector < 0)
    {
      // moving left
      strip->setPixelColor(abs(sector), currentColor);
      DBPRINT(abs(sector));
    }
    else
    {
      // moving right
      strip->setPixelColor(sector, currentColor);
      DBPRINT(sector);
    }
    sector++;
    if (sector == SECTOR_COUNT)
      sector = -(SECTOR_COUNT-1);

    scannerUpdateTime = millis() + SCAN_DELAY_MS;
  }
  DBPRINT(", ");DBPRINTLN(currentColor);
  strip->show();
}

void Lights::blank()
{
  for (int i = 0; i < SECTOR_COUNT; i++)
  {
    strip->setPixelColor(i, 0);
  }
}
