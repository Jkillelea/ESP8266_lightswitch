# ESP8266 Lightswitch [![Build Status](https://travis-ci.org/Jkillelea/ESP8266_lightswitch.svg?branch=master)](https://travis-ci.org/Jkillelea/ESP8266_lightswitch)
## Turns my lights on and off via an HTML request

Not much more to add than that. In the future I'd like to set an interrupt pin
(if the 8266 can do that) so the lights can be turned on and off with a button too.

Right now it's switching some LEDs with a MOSFET but the general idea transfers
over to anything, more or less.

------------------

The codebase is written in Arduino C++ and it currently builds for the esp01 and nodemcu v2
with PlatformIO.

------------------

Currently, there are only two functionalities in the chip:
requesting `hostname.local/` will toggle the lights on and off and return a 1 or 0 to represent the new state (high or low)

requesting `hostname.local/get` returns a 1 or 0, depending on the state of the pin (high or low)
