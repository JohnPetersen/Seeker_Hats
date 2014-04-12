
#include <Wire.h>
#include <Adafruit_LSM303.h>

Adafruit_LSM303 compass;

Adafruit_LSM303::lsm303MagData calMin;
Adafruit_LSM303::lsm303MagData calMax;

void setupCompass()
{
    if (!compass.begin())
  {
    Serial.println("Oops ... unable to initialize the compass. Check your wiring!");
    while (1);
  }
  // TODO set actual compass callibration values depending on the device.
  calMin.x = 0.0;
  calMin.y = 0.0;
  calMin.z = 0.0;
  calMax.x = 0.0;
  calMax.y = 0.0;
  calMax.z = 0.0;
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
  float heading = getHeading();
  char* compassPoint = headingToCompassPoint(heading);
  Serial.print("Heading: "); Serial.print(compassPoint); Serial.print(" : "); Serial.println(heading);
  Serial1.print("Heading: "); Serial1.print(compassPoint); Serial1.print(" : "); Serial1.println(heading);
}

