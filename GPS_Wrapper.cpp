#include "GPS_Wrapper.h"
  
  GPS_Wrapper::GPS_Wrapper(WorldState* ws)
  {
    myState = ws;
    SoftwareSerial* gpsSerial = new SoftwareSerial(PIN_GPS_TX, PIN_GPS_RX);
    GPS = new Adafruit_GPS(gpsSerial);
  }
  
  void GPS_Wrapper::begin()
  { 
    // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
    GPS->begin(9600);
    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    //GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    GPS->sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
    // the parser doesn't care about other sentences at this time
    // Set the update rate
    GPS->sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    // For the parsing code to work nicely and have time to sort thru the data, and
    // print it out we don't suggest using anything higher than 1 Hz
       
    // Request updates on antenna status, comment out to keep quiet
    //GPS->sendCommand(PGCMD_ANTENNA);
   
    // TODO Turn on logging.
    DBPRINTLN("STARTING LOGGING....");
    if (GPS->LOCUS_StartLogger())
    {  
      DBPRINTLN(" STARTED!");
    } 
    else
    {
      DBPRINTLN(" no response :(");
    }

    delay(1000);
    // Ask for firmware version
    GPS->sendCommand(PMTK_Q_RELEASE);
    
    DEBUG_PRINT("GPS Setup Complete!");
  }
  
  void GPS_Wrapper::processGps()
  {
    char c = GPS->read();
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS->newNMEAreceived()) {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
      //Serial.println(GPS->lastNMEA()); // this also sets the newNMEAreceived() flag to false
      if (!GPS->parse(GPS->lastNMEA())) // this also sets the newNMEAreceived() flag to false
      {
        DBPRINT("Failed to parse: ");
        DBPRINT(GPS->lastNMEA());
      }
    }
  }
  
  long GPS_Wrapper::toBAMS(float v, char d)
  {
    float degrees = floor(v / 100.0);
    float minutes = v - (degrees * 100);
    long bams = ((degrees / 180.0) * MAX_LONG) + ((minutes / (60.0 * 180.0)) * MAX_LONG);
    return d == 'S' || d == 'W' ? -bams : bams;
  }
  
  void GPS_Wrapper::updatePosition()
  {
    // TODO This method is responsible for:
    // 1. converting the GPS coordinates into BAMS
    // 2. storing the coordinates locally
    // 3. sending them to the other device
    
#ifdef DEBUG
    DBPRINT("\nTime: ");
    DBPRINT(GPS->hour); DBPRINT(':');
    DBPRINT(GPS->minute); DBPRINT(':');
    DBPRINT(GPS->seconds); DBPRINT('.');
    DBPRINTLN(GPS->milliseconds);
    DBPRINT("Date: ");
    DBPRINT(GPS->day); DBPRINT('/');
    DBPRINT(GPS->month); DBPRINT("/20");
    DBPRINTLN(GPS->year);
    DBPRINT("Fix: "); DBPRINT((int)GPS->fix);
    DBPRINT(" quality: "); DBPRINTLN((int)GPS->fixquality);
#endif    
    if (GPS->fix) {
#ifdef DEBUG
      DBPRINT("Location: ");
      DBPRINT(GPS->latitude); DBPRINT(GPS->lat);
      DBPRINT(", ");
      DBPRINT(GPS->longitude); DBPRINTLN(GPS->lon);
      DBPRINT("Speed (knots): "); DBPRINTLN(GPS->speed);
      DBPRINT("Angle: "); DBPRINTLN(GPS->angle);
      DBPRINT("Altitude: "); DBPRINTLN(GPS->altitude);
      DBPRINT("Satellites: "); DBPRINTLN((int)GPS->satellites);
#endif
      // 1. convert lat & lon to BAMS
      long latBAMS = toBAMS(GPS->latitude, GPS->lat);
      long lonBAMS = toBAMS(GPS->longitude, GPS->lon);
      
      // 2. update the state
      myState->lat = latBAMS;
      myState->lon = lonBAMS;
      myState->lastUpdateTime = millis();
      myState->setFixFlag(true);  
    }
    else
    {
      myState->setFixFlag(false);
    }
    // 3. send position to the other
    Serial1.write(myState->getMessage(), MSG_LEN);
    DBPRINT("Sending {");DBPRINT(myState->alarm);DBPRINT(",");DBPRINT(myState->lat);DBPRINT(",");DBPRINT(myState->lon);DBPRINTLN("}");
  }
