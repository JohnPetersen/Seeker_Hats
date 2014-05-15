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

  float a = atan2(y, x);
  a *= RAD2DEG;

  // rotate so North is 0 degrees and flip so west (CCW) is negative
  a = -(a - 90.0);
  if (a < 0) a += 360.0;

  DBPRINTLN("Sector::calculate()");
  DBPRINT("   azimuth: ");DBPRINTLN(a);
  DBPRINT("   heading: ");DBPRINTLN(myState->heading);

  // The difference between the azimuth and our heading is the 
  // line of bearing to the other device.
  a -= myState->heading;
  if (a < -180.0)
    a += 360.0;
  if (a > 180.0)
    a -= 360.0;

  DBPRINT("   bearing: ");DBPRINTLN(a);

  // The sector is just a binning of the bearings.
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

  DBPRINT("   sector : ");DBPRINTLN(sctr);
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
