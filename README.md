# Seeker Hats 

## What are Seeker Hats
A pair of hats that point to each other. Allow the wearers of the hats to find one another.

TODO: Add images of the hats

## Motivation
For the 2013 Maker Faire I built a blinky light hat for my son to wear. It used a Flora, a LSM303 compass and eight WS2812 LEDs. Rather than making a hat with more LEDs for 2014 I decided to make something useful. Based on our differing interests I thought a pair of hats that would let us separate yet give us the ability to locate each other quickly would be useful.

## Parts Used
Each hat contains the following parts:
- [Spark Fun Fio v3](https://www.sparkfun.com/products/11520)
- [xBee Pro S1](https://www.sparkfun.com/products/8742)
- [Adafruit Ultimate GPS](https://www.adafruit.com/products/746)
- [Adafruit Flora Compass](https://www.adafruit.com/product/1247)
- [Adafruit Flora Neopixels (5x)](https://www.adafruit.com/products/1260)
- [2500 mAh lipo cell](https://www.adafruit.com/products/328)

## How it works
### Hardware
1. GPS receiver on hat determines location.
2. xBee is used to exchange location data with the other hat.
3. Compass is used to determine the orientation of each hat.
4. The Fio computes the azimuth to the other hat and the difference from its heading.
5. The Neopixel in the direction of the other hat is illuminated.

### Software
1. The GPS receiver is queried every two seconds to get the current position.
2. When a position is read from the GPS it is sent to the other hat.
3. The other hat's position is received.
4. The current heading is read from the compass and the sector containing the other hat is calculated.
5. The lights are updated to reflect any changes in the sector.

TODO: Add class descriptions and sequence diagram

#### Simulator
Processing was used to simulate a second hat durring developments. This enabled the verification of the communications and algorithm implemenations.

TODO: Add simulator screen shot

#### Dependencies
The following libraries are used:
- [Adafruit_GPS](https://github.com/adafruit/Adafruit-GPS-Library)
- [Adafruit_LSM303](https://github.com/adafruit/Adafruit_LSM303)
- [Adafruit_NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel)
- [EEPROM](http://arduino.cc/en/Reference/EEPROM)
- [SoftwareSerial](http://arduino.cc/en/Reference/SoftwareSerial)
- [TimedAction](http://playground.arduino.cc/Code/TimedAction)
- [Wire](http://arduino.cc/en/Reference/Wire)

## Problems
Issues with the current version:
- My son would turn off his hat.
- Only two people can participate.
- Brim of hat not the easist to see.
- A button was originally part of the system. The button was designed to allow for signalling between the two devices. The design prototype worked hsowever in the final build pressing the button caused the device to reset so the button was removed. 
- The GPS module has a built-in logging capability, but code to activate it froze the program.

## Future Work
Improvement ideas for the next version:
- Mesh network with more than two devices.
- Create a held device that uses an [Adafruit NeoPixel Ring](https://www.adafruit.com/products/1586) to point to the hats.
- Fix the alert button.
