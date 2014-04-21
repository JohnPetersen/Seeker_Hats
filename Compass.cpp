#include "Compass.h"

Compass::Compass(WorldState* ms)
{
  myState = ms;
  compass = new Adafruit_LSM303();
  
  // TODO set actual compass callibration values depending on the device.
  calMin.x = 0.0;
  calMin.y = 0.0;
  calMin.z = 0.0;
  calMax.x = 0.0;
  calMax.y = 0.0;
  calMax.z = 0.0;
}

void Compass::begin()
{
  if (!compass->begin())
  {
    DBPRINTLN("Oops ... unable to initialize the compass. Check your wiring!");
    while (1);
  }
  
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
float Compass::getHeading()
{
  vector<int> from = (vector<int>){0, -1, 0};
  
  compass->read();
  
  vector<int32_t> temp_m = {compass->magData.x, compass->magData.y, compass->magData.z};
  vector<int32_t> temp_a = {compass->accelData.x, compass->accelData.y, compass->accelData.z};


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

void Compass::update()
{
  // TODO 1. read current heading
  //      2. Calculate which sector the other is in
  //      3. Use a two vote state machine to debounce changes in sectors
  //      4. Update the lights
  float heading = getHeading();
//  char* compassPoint = headingToCompassPoint(heading);
//  Serial.print("Heading: "); Serial.print(compassPoint); Serial.print(" : "); Serial.println(heading);
//  Serial1.print("Heading: "); Serial1.print(compassPoint); Serial1.print(" : "); Serial1.println(heading);
}

