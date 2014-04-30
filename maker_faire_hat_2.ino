
#include <Wire.h>
#include <TimedAction.h>

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>

#include "debug_utils.h"
#include "Sector.h"
#include "WorldState.h"
#include "GPS_Wrapper.h"
#include "Compass.h"
#include "Lights.h"

#define BUTTON_INTERRUPT 4
#define DEBOUNCE_TIME 500

TimedAction actUpdatePosition = TimedAction(2000, positionUpdateTask);
TimedAction actReceivePosition = TimedAction(1000, receiveOtherStateTask);
TimedAction actHeading = TimedAction(250, headingTask);

Sector* sector;
GPS_Wrapper* gpsSensor;
Compass* compass;
Lights* lights;
WorldState* myState;
WorldState* otherState;

volatile bool buttonPushed = false;
volatile long nextPushAllowedTime = 0;

void setup()
{
  delay(10000);
  INIT_DEBUG();
  Serial1.begin(9600); //This is the UART, pipes to the XBee

  DBPRINTLN("Init WorldState...");
  myState = new WorldState();
  otherState = new WorldState();

  DBPRINTLN("Init GPS_Wrapper...");
  gpsSensor = new GPS_Wrapper(myState);
  gpsSensor->begin();
  
  DBPRINTLN("Init Compass...");
  compass = new Compass(myState);
  compass->begin();
  
  DBPRINTLN("Init Lights...");
  lights = new Lights(myState, otherState);
  lights->begin();

  DBPRINTLN("Init Sector...");
  sector = new Sector(myState, otherState);
  
  DBPRINTLN("Init button interrupt...");
  attachInterrupt(BUTTON_INTERRUPT, buttonHandler, RISING);

  DEBUG_PRINT("Setup Complete!");
}

void loop() // run over and over again
{
  // read data from the GPS in the 'main loop'
  gpsSensor->processGps();
  
  // check for button pushes
  if (buttonPushed) {
    DBPRINTLN("Button PUSH");
    // this button press was an ack if the other had already pushed their button
    if (otherState->isButtonFlagSet() && !myState->isAckFlagSet())
      myState->setAckFlag(true);
    else
      myState->setButtonFlag(true);
    
    buttonPushed = false;
  }

  actUpdatePosition.check();
  actReceivePosition.check();
  actHeading.check();

  lights->update();
}

void positionUpdateTask()
{
  // read my position from GPS, update the state, and transmit it to the other.
  gpsSensor->updatePosition();
}

void receiveOtherStateTask()
{
  // receive the other's position and update the state
  if (Serial1.available() > 0) {
    bool isOtherAlarmed = otherState->isButtonFlagSet();
    byte buff[MSG_LEN] = { 0 };
    // TODO check the number of bytes read...
    Serial1.readBytes((char*)buff, MSG_LEN);
    otherState->update(buff);
    DBPRINT("received: ");DBPRINT(otherState->alarm);DBPRINT(",");
    DBPRINT(otherState->lat);DBPRINT(",");DBPRINTLN(otherState->lon);
    
    if (isOtherAlarmed && !otherState->isButtonFlagSet())
      myState->setAckFlag(false);
    if (otherState->isAckFlagSet())
      myState->setButtonFlag(false);
  }
}

void headingTask()
{
  compass->update();
  // TODO update the lights.
  // 1. calculate the other's sector based on our current and received positions.
  
  // 2. if this is a new quadrant update the lights.
  byte currSector = sector->getCurrent();
  if (currSector != ERROR_SECTOR)
    lights->setSector(currSector);
}

void buttonHandler() {
  if (millis() > nextPushAllowedTime) {
    buttonPushed = true;
    nextPushAllowedTime = millis() + DEBOUNCE_TIME;
  }
}
