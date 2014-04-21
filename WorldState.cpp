#include "WorldState.h"

// TODO test the conversion to and from the byte array

WorldState::WorldState()
{
  lat = lon = 0;
  alarm = 0;
}

long WorldState::getLong(byte buff[]) {
  long l = buff[0];
  l = l << 8 | buff[1];
  l = l << 8 | buff[2];
  l = l << 8 | buff[3];
  return l;
}

void WorldState::setLong(byte buff[], long l) {
  buff[0] = (int)((l >> 24) & 0xFF);
  buff[1] = (int)((l >> 16) & 0xFF);
  buff[2] = (int)((l >> 8) & 0XFF);
  buff[3] = (int)((l & 0XFF));
}

void WorldState::getMessage(byte buff[]) {
  buff[0] = MSG_TAG;
  buff[1] = alarm;
  setLong(buff + 2, lat);
  setLong(buff + 6, lon);
  buff[10] = MSG_END;
}

boolean WorldState::update(byte buff[], State* s) {
  if (buff[0] != MSG_TAG) return false;
  s->alarm = buff[1];
  s->lat = getLong(buff + 2);
  s->lon = getLong(buff + 6);
  return true;
}
