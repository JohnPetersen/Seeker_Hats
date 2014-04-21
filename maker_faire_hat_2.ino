
#define DEBUG
#include <debug_utils.h>

#include <Wire.h>
#include <TimedAction.h>

#include <debug_utils.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#include <Adafruit_LSM303.h>
#include <Adafruit_NeoPixel.h>

#include "WorldState.h"
#include "GPS_Wrapper.h"

#define PIN_LIGHTS 5



namespace Lights
{
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(3, PIN_LIGHTS, NEO_GRB + NEO_KHZ800);
  
  void setupLights()
  { 
  //  strip.begin();
  //  strip.show(); // Initialize all pixels to 'off'
  
    DEBUG_PRINT("Lights Setup Complete!");
  }
} // end Lights

namespace Compass
{
  Adafruit_LSM303 compass;

  Adafruit_LSM303::lsm303MagData calMin;
  Adafruit_LSM303::lsm303MagData calMax;
  
  void setupCompass()
  {
      if (!compass.begin())
    {
      DBPRINTLN("Oops ... unable to initialize the compass. Check your wiring!");
      while (1);
    }
    // TODO set actual compass callibration values depending on the device.
    calMin.x = 0.0;
    calMin.y = 0.0;
    calMin.z = 0.0;
    calMax.x = 0.0;
    calMax.y = 0.0;
    calMax.z = 0.0;
    
    DEBUG_PRINT("Compass Setup Complete!");
  }
  
  template <typename T> struct vector
  {
    T x, y, z;
  };
  
  template <typename Ta, typename Tb, typename To> void vector_cross(const vector<Ta> *a,const vector<Tb> *b, vector<To> *out)
  {
    out->x = (a->y * b->z) - (a->z * b->y);
    out->y = (a->z * b->x) - (a->x * b->z);
    out->z = (a->x * b->y) - (a->y * b->x);
  }
  
  template <typename Ta, typename Tb> float vector_dot(const vector<Ta> *a, const vector<Tb> *b)
  {
    return (a->x * b->x) + (a->y * b->y) + (a->z * b->z);
  }
  
  void vector_normalize(vector<float> *a)
  {
    float mag = sqrt(vector_dot(a, a));
    a->x /= mag;
    a->y /= mag;
    a->z /= mag;
  }
  
  /*
  Returns the angular difference in the horizontal plane between the
  "from" vector and north, in degrees.
  
  Description of heading algorithm:
  Shift and scale the magnetic reading based on calibration data to find
  the North vector. Use the acceleration readings to determine the Up
  vector (gravity is measured as an upward acceleration). The cross
  product of North and Up vectors is East. The vectors East and North
  form a basis for the horizontal plane. The From vector is projected
  into the horizontal plane and the angle between the projected vector
  and horizontal north is returned.
  */
  float getHeading()
  {
    vector<int> from = (vector<int>){0, -1, 0};
    
    compass.read();
    
    vector<int32_t> temp_m = {compass.magData.x, compass.magData.y, compass.magData.z};
    vector<int32_t> temp_a = {compass.accelData.x, compass.accelData.y, compass.accelData.z};
  
  
    // subtract offset (average of min and max) from magnetometer readings
    temp_m.x -= ((int32_t)calMin.x + calMax.x) / 2;
    temp_m.y -= ((int32_t)calMin.y + calMax.y) / 2;
    temp_m.z -= ((int32_t)calMin.z + calMax.z) / 2;
  
    // compute E and N
    vector<float> E;
    vector<float> N;
    vector_cross(&temp_m, &temp_a, &E);
    vector_normalize(&E);
    vector_cross(&temp_a, &E, &N);
    vector_normalize(&N);
  
    // compute heading
    float heading = atan2(vector_dot(&E, &from), vector_dot(&N, &from)) * 180 / M_PI;
    if (heading < 0) heading += 360;
    return heading;
  }
  
  char* headingToCompassPoint(float h)
  {
    if (h > 337.5 || h <= 22.5)
      return " N";
    if (h > 22.5 && h <= 67.5)
      return "NE";
    if (h > 67.5 && h <= 112.5)
      return " E";
    if (h > 112.5 && h <= 157.5)
      return "SE";
    if (h > 157.5 && h <= 202.5)
      return " S";
    if (h > 202.5 && h <= 247.5)
      return "SW";
    if (h > 247.5 && h <= 292.5)
      return " W";
    if (h > 292.5 && h <= 337.5)
      return "NW";
  }
  
  void updateHeading()
  {
    // TODO 1. read current heading
    //      2. Calculate which sector the other is in
    //      3. Use a two vote state machine to debounce changes in sectors
    //      4. Update the lights
    float heading = getHeading();
    char* compassPoint = headingToCompassPoint(heading);
    Serial.print("Heading: "); Serial.print(compassPoint); Serial.print(" : "); Serial.println(heading);
    Serial1.print("Heading: "); Serial1.print(compassPoint); Serial1.print(" : "); Serial1.println(heading);
  }
} // end Compass


TimedAction actUpdatePosition = TimedAction(2000, positionUpdateTask);
TimedAction actReceivePosition = TimedAction(1000, positionReceiveTask);
TimedAction actHeading = TimedAction(250, headingTask);

GPS_Wrapper* gpsSensor;
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
  
  Compass::setupCompass();
  Lights::setupLights();
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
  Compass::updateHeading();
  // TODO update the lights.
  // 1. calculate the other's sector based on our current and received positions.
  // 2. if this is a new quadrant update the lights. 
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
  }
}

