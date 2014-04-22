#include "WorldState.h"

// TODO test the conversion to and from the byte array

WorldState::WorldState()
{
  lat = lon = 0;
  alarm = 0;
  lastUpdateTime = 0;
  heading = 0.0;
}

long WorldState::getLong(byte buff[])
{
  long l = buff[0];
  l = l << 8 | buff[1];
  l = l << 8 | buff[2];
  l = l << 8 | buff[3];
  return l;
}

void WorldState::setLong(byte buff[], long l)
{
  buff[0] = (int)((l >> 24) & 0xFF);
  buff[1] = (int)((l >> 16) & 0xFF);
  buff[2] = (int)((l >> 8) & 0XFF);
  buff[3] = (int)((l & 0XFF));
}

byte* WorldState::getMessage()
{
  msgBuff[0] = MSG_TAG;
  msgBuff[1] = alarm;
  setLong(msgBuff + 2, lat);
  setLong(msgBuff + 6, lon);
  msgBuff[10] = MSG_END;
  return msgBuff;
}

boolean WorldState::update(byte buff[])
{
  if (buff[0] != MSG_TAG) return false;
  alarm = buff[1];
  lat = getLong(buff + 2);
  lon = getLong(buff + 6);
  return true;
}
