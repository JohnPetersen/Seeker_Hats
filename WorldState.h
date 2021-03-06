#ifndef _WORLDSTATE_H
#define _WORLDSTATE_H

#include "Arduino.h"

#define MSG_LEN 11
#define MSG_TAG 0xff
#define MSG_END 0x00

class WorldState
{
  public:
    WorldState();
    byte alarm;
    long lat;
    long lon;
    unsigned long lastUpdateTime;
    float heading;
    // fill the buffer with a state message
    byte* getMessage(void);
    // update this state with the contents of the buffer
    boolean update(byte buff[]);
    void setButtonFlag(bool b);
    void setAckFlag(bool b);
    void setFixFlag(bool b);
    bool isButtonFlagSet(void);
    bool isAckFlagSet(void);
    bool isFixFlagSet(void);
  private:
    byte msgBuff[MSG_LEN];
    long getLong(byte buff[]);
    void setLong(byte buff[], long l);
};

#endif
