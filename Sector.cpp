#include "Sector.h"

Sector::Sector(WorldState* ms, WorldState* os)
{
  myState = ms;
  otherState = os;
}

byte Sector::getCurrent()
{
  if (myState->isFixFlagSet() && otherState->isFixFlagSet())
    return smooth(calculate());
  else
    return ERROR_SECTOR;
}

byte Sector::calculate()
{
  int x = otherState->lon - myState->lon;
  int y = otherState->lat - myState->lat;

  float a = atan2(x, y);
  a *= RAD2DEG;

  DBPRINTLN("Sector::calculate()");
  DBPRINT("   azimuth to other: ");DBPRINTLN(a);

  a = -(a - 90.0);
  if (a < -180.0)
    a += 360.0;

  DBPRINT("   relative to my heading: ");DBPRINTLN(a);

  byte sctr = ERROR_SECTOR;
  if (a <= -90)
    sctr = 0;
  else if (a < -10 && a > -90)
    sctr = 1;
  else if (a <= 10 && a >= -10)
    sctr = 2;
  else if (a > 10 && a < 90)
    sctr = 3;
  else if (a >= 90)
    sctr = 4;

  DBPRINT("   calculated sector: ");DBPRINTLN(sctr);
  return sctr;
}

byte Sector::smooth(byte s)
{
  // Require two consecutive sectors to change
  if (s == current)
  {
    candidate = current;
    return current;
  }
  if (s == candidate)
  {
    current = s;
  }
  else
  {
    candidate = s;
  }
  return current;
}