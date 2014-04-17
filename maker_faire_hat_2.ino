
#include <TimedAction.h>

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
  updatePosition();
}

void positionReceiveTask()
{
  // TODO Need to receive the other's position
  //receiveOtherPosition();
}

void headingTask()
{
  updateHeading();
  // TODO update the lights.
  // 1. calculate the other's sector based on our current and received positions.
  // 2. if this is a new quadrant update the lights. 
}
