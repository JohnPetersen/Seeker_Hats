#include "Lights.h"

Lights::Lights(WorldState* ms, WorldState* os)
{
  myState = ms;
  otherState = os;
  sector = 99;
  sectorUpdated = false;
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

  DBPRINTLN("LED Colors:");
  DBPRINT("   Red: ");DBPRINTLN(red);
  DBPRINT("   Green: ");DBPRINTLN(green);
  DBPRINT("   Blue: ");DBPRINTLN(blue);
  DEBUG_PRINT("Lights Setup Complete!");
}

void Lights::setSector(byte s)
{
  // TODO update the light status based on the current states.
  if (s != sector) {
    sector = s;
    sectorUpdated = true;
  }
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
  // Choose update style based on validity of our state. Do we have fixes?
  if (myState->isFixFlagSet() && otherState->isFixFlagSet())
  {
    if (sectorUpdated) {
      sectorUpdated = false;
      blank();
      // We have good fixes, display the pointer
      strip->setPixelColor(sector, currentColor);
      DBPRINT("LIGHT: ");DBPRINT(sector);DBPRINT("(fixed), ");DBPRINTLN(currentColor);
      strip->show();
    }
  }
  else if (millis() > scannerUpdateTime)
  {
    DBPRINT("LIGHT: ");
    blank();
    // Missing one or both fixes, display a larson scanner
    if (sector >= SECTOR_COUNT)
      sector = -(SECTOR_COUNT-2);
    if (sector < 0)
    {
      // moving left
      strip->setPixelColor(abs(sector), currentColor);
      DBPRINT(abs(sector));DBPRINT("(left)");
    }
    else
    {
      // moving right
      strip->setPixelColor(sector, currentColor);
      DBPRINT(sector);DBPRINT("(right)");
    }
    sector++;
    DBPRINT(", ");DBPRINTLN(currentColor);
    strip->show();
    scannerUpdateTime = millis() + SCAN_DELAY_MS;
  }
}

void Lights::blank()
{
  for (int i = 0; i < SECTOR_COUNT; i++)
  {
    strip->setPixelColor(i, 0);
  }
}
