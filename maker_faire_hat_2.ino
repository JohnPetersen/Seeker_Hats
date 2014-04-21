#include <TimedAction.h>

#define MSG_LEN 11
#define MSG_TAG 0xff
#define MSG_END 0x00

namespace WorldState {
  struct State {
    byte alarm;
    long lat;
    long lon;
  };
  
  State my = { 0, 0, 0 };
  State other = { 0, 0, 0 };
  
  // TODO test the conversion to and from the byte array
  
  long getLong(byte buff[]) {
    long l = buff[0];
    l = l << 8 | buff[1];
    l = l << 8 | buff[2];
    l = l << 8 | buff[3];
    return l;
  }

  void setLong(byte buff[], long l) {
    buff[0] = (int)((l >> 24) & 0xFF);
    buff[1] = (int)((l >> 16) & 0xFF);
    buff[2] = (int)((l >> 8) & 0XFF);
    buff[3] = (int)((l & 0XFF));
  }
  
  void get(byte buff[], State* s) {
    buff[0] = MSG_TAG;
    buff[1] = s->alarm;
    setLong(buff + 2, s->lat);
    setLong(buff + 6, s->lon);
    buff[10] = MSG_END;
  }
  
  boolean set(byte buff[], State* s) {
    if (buff[0] != MSG_TAG) return false;
    s->alarm = buff[1];
    s->lat = getLong(buff + 2);
    s->lon = getLong(buff + 6);
    return true;
  }
}

TimedAction actUpdatePosition = TimedAction(2000, positionUpdateTask);
TimedAction actReceivePosition = TimedAction(1000, positionReceiveTask);
TimedAction actHeading = TimedAction(250, headingTask);

void setup()
{
  Serial.begin(115200); // Connected to USB
  Serial1.begin(9600); //This is the UART, pipes to the XBee

  setupGps();
  setupCompass();
  setupLights();
}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  processGps();
  
  actUpdatePosition.check();
  actReceivePosition.check();
  actHeading.check();
}

void positionUpdateTask()
{
  // read my position from GPS, update the state, and transmit it to the other.
  updatePosition();
}

void positionReceiveTask()
{
  // receive the other's position and update the state
  receiveMessage();
}

void headingTask()
{
  updateHeading();
  // TODO update the lights.
  // 1. calculate the other's sector based on our current and received positions.
  // 2. if this is a new quadrant update the lights. 
}

void receiveMessage() {
  byte buff[MSG_LEN] = { 0 };
  long tmpLat = 0;
  long tmpLon = 0;
  if (Serial1.available() > 0) {
    // TODO check the number of bytes read...
    Serial.readBytes((char*)buff, MSG_LEN);
    WorldState::set(buff, &WorldState::other);
  }
}

