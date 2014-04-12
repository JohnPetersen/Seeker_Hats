#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

int gpsTx = 8;
int gpsRx = 7;

SoftwareSerial gpsSerial(gpsTx, gpsRx);

Adafruit_GPS* GPS;
     
// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false
     
// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;

void setupGps()
{
  GPS = new Adafruit_GPS(&gpsSerial);
  
   // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS->begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
     
  // Request updates on antenna status, comment out to keep quiet
  GPS->sendCommand(PGCMD_ANTENNA);
 
  delay(1000);
  // Ask for firmware version
  gpsSerial.println(PMTK_Q_RELEASE);
}

void processGps()
{
  char c = GPS->read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS->newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS->lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS->parse(GPS->lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }
}

void updatePosition()
{
  Serial.print("\nTime: ");
  Serial.print(GPS->hour, DEC); Serial.print(':');
  Serial.print(GPS->minute, DEC); Serial.print(':');
  Serial.print(GPS->seconds, DEC); Serial.print('.');
  Serial.println(GPS->milliseconds);
  Serial.print("Date: ");
  Serial.print(GPS->day, DEC); Serial.print('/');
  Serial.print(GPS->month, DEC); Serial.print("/20");
  Serial.println(GPS->year, DEC);
  Serial.print("Fix: "); Serial.print((int)GPS->fix);
  Serial.print(" quality: "); Serial.println((int)GPS->fixquality);
  if (GPS->fix) {
    Serial.print("Location: ");
    Serial.print(GPS->latitude, 4); Serial.print(GPS->lat);
    Serial.print(", ");
    Serial.print(GPS->longitude, 4); Serial.println(GPS->lon);
    Serial.print("Speed (knots): "); Serial.println(GPS->speed);
    Serial.print("Angle: "); Serial.println(GPS->angle);
    Serial.print("Altitude: "); Serial.println(GPS->altitude);
    Serial.print("Satellites: "); Serial.println((int)GPS->satellites);
    
    Serial1.print("Location: ");
    Serial1.print(GPS->latitude, 4); Serial1.print(GPS->lat);
    Serial1.print(", ");
    Serial1.print(GPS->longitude, 4); Serial1.println(GPS->lon);
    Serial1.print("Speed (knots): "); Serial1.println(GPS->speed);
    Serial1.print("Angle: "); Serial1.println(GPS->angle);
    Serial1.print("Altitude: "); Serial1.println(GPS->altitude);
    Serial1.print("Satellites: "); Serial1.println((int)GPS->satellites);
  }
}
