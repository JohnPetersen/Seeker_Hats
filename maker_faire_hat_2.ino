

void setup()
{
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");
  
  Serial1.begin(9600); //This is the UART, pipes to the XBee

  setupGps();
  setupCompass();
  setupLights();
}
     


uint32_t timer = millis();
void loop() // run over and over again
{
  
  // if millis() or timer wraps around, we'll just reset it
  if (timer > millis()) timer = millis();
  
  // read data from the GPS in the 'main loop'
  processGps();
     
  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    // GPS
    updatePosition();
    // Compass
    updateHeading();
    
    timer = millis(); // reset the timer
  }
}
