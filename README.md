# Seeker Hats 

## What are Seeker Hats?
Seeker hats are a pair of hats that can tell the wearer the direction of the other hat. This allows the wearers of the hats to find one another.

![Overview](https://raw.githubusercontent.com/JohnPetersen/Seeker_Hats/master/docs/img/overview.jpg)

The hats use an array of LEDs mounted under the brim to indicate the direction of the other hat.

![LED Array](https://raw.githubusercontent.com/JohnPetersen/Seeker_Hats/master/docs/img/lights.jpg)

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

![Hardware Detail](https://raw.githubusercontent.com/JohnPetersen/Seeker_Hats/master/docs/img/detail.png)

### Software
1. The GPS receiver is queried every two seconds to get the current position.
2. When a position is read from the GPS it is sent to the other hat.
3. The other hat's position is received.
4. The current heading is read from the compass and the sector containing the other hat is calculated.
5. The lights are updated to reflect any changes in the sector.

TODO: Add class descriptions and sequence diagram

#### Structure
The software uses the Arduino .ino file standard for the setup() and loop() functions. The majority of the functionality is provided by classes instantiated in the setup() method and used during the loop() calls.

- Compass: Wraps the basic Adafruit_LSM303 functionality and provides a tilt corrected heading.
- GPS_Wrapper: Wraps the Adafruit_GPS library with logic to convert the string representation to [BAMS](http://en.wikipedia.org/wiki/Binary_Angular_Measurement_System#Binary_angles), update the local state, and send the location to the other hat.
- Lights: Wraps the Adafruit_NeoPixel library and decides how to illuminate the lights based on the states of the two hats.
- WorldState: A structure to hold the state of a hat. Encapsulates the logic for serializing a state to and from a byte array for transmitting and receiving.
- Sector: Utility class for calculating which sector the other hat is in.

#### Simulator
Processing was used to simulate a second hat during development. This enabled the verification of the communications and algorithm implementations. The simulator can be found in [this repository](https://github.com/JohnPetersen/Seeker_Hat_Simulator).

![Simulator Screenshot](https://raw.githubusercontent.com/JohnPetersen/Seeker_Hats/master/docs/img/simulator.png)

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
- Brim of hat not the easiest to see.
- A button was originally part of the system. The button was designed to allow for signaling between the two devices. The design prototype worked however in the final build pressing the button caused the device to reset so the button was removed. 
- The GPS module has a built-in logging capability, but code to activate it froze the program.

## Future Work
Improvement ideas for the next version:

- Use a mesh network to allow more than two devices to participate. The hats could point to the nearest device.
- Create a handheld device that uses an [Adafruit NeoPixel Ring](https://www.adafruit.com/products/1586) to point to the hats.
- Fix the alert button.

## Links
[Hackaday](https://hackaday.com/2014/06/18/seeker-hats-find-each-other-with-directional-leds/)

