
#include <debug_utils.h>

#include <Wire.h>
#include <TimedAction.h>

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>

#include "WorldState.h"
#include "GPS_Wrapper.h"
#include "Compass.h"
#include "Lights.h"

TimedAction actUpdatePosition = TimedAction(2000, positionUpdateTask);
TimedAction actReceivePosition = TimedAction(1000, positionReceiveTask);
TimedAction actHeading = TimedAction(250, headingTask);

GPS_Wrapper* gpsSensor;
Compass* compass;
Lights* lights;
WorldState* myState;
WorldState* otherState;

void setup()
{
  INIT_DEBUG();
  Serial1.begin(9600); //This is the UART, pipes to the XBee

  myState = new WorldState();
  otherState = new WorldState();

  gpsSensor = new GPS_Wrapper(myState);
  gpsSensor->begin();
  
  compass = new Compass(myState);
  compass->begin();
  
  lights = new Lights(myState, otherState);
  lights->begin();
  
  DEBUG_PRINT("Setup Complete!");
}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  gpsSensor->processGps();
  
  actUpdatePosition.check();
  actReceivePosition.check();
  actHeading.check();
}

void positionUpdateTask()
{
  // read my position from GPS, update the state, and transmit it to the other.
  gpsSensor->updatePosition();
}

void positionReceiveTask()
{
  // receive the other's position and update the state
  receiveMessage();
}

void headingTask()
{
  compass->update();
  // TODO update the lights.
  // 1. calculate the other's sector based on our current and received positions.
  int sector = calcSector();
  // 2. if this is a new quadrant update the lights. 
  lights->update();
}

int calcSector()
{
  // TODO use the lat-lon from the two WorldStates and the heading to determine 
  //   which sector the other device is in.
  //   Debounce the sector...

  float a = atan2(1, 0);

  return floor(a);
}

void receiveMessage() {
  // Receive message from other device
  byte buff[MSG_LEN] = { 0 };
  long tmpLat = 0;
  long tmpLon = 0;
  if (Serial1.available() > 0) {
    // TODO check the number of bytes read...
    Serial1.readBytes((char*)buff, MSG_LEN);
    otherState->update(buff);
    DBPRINT("received: ");DBPRINT(otherState->alarm);DBPRINT(",");DBPRINT(otherState->lat);DBPRINT(",");DBPRINT(otherState->lon);
  }
}

