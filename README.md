thermo-js 
===

An open-source DIY implementation of a thermostat, controllable from home and smartphone.


What you need
---

Based on a raw and optimistic count, you should spend around 120$ and some hours to set it up.

Hardware:

* Arduino (I've used 2009) http://store.arduino.cc/ with a caompatible usb cable (the printer one worked well for me)
* A pc or a rasperrypi (http://www.raspberrypi.org/) with a debian installed
* A Seed Studio Groove board and some components. 
I've initially used this http://www.seeedstudio.com/depot/Grove-Starter-Kit-p-709.html but has been discontinued.. There is a "plus" one, but the serial LCD is missing.
 
Single components used:

* Base shield http://www.seeedstudio.com/depot/base-shield-v13-p-1378.html?cPath=132_134
* SerialLCD http://www.seeedstudio.com/depot/grove-serial-lcd-p-773.html?cPath=163_164
* Relay http://www.seeedstudio.com/depot/grove-relay-p-769.html?cPath=156_160
* Temperature sensor http://www.seeedstudio.com/depot/grove-temperature-sensor-p-774.html?cPath=144_147
* LED http://www.seeedstudio.com/depot/grove-led-p-767.html?cPath=156_157
* Rotary Angle Sensor http://www.seeedstudio.com/depot/grove-rotary-angle-sensor-p-p-1242.html?cPath=156_160
* Some connection cables http://www.seeedstudio.com/depot/Grove-Universal-4-Pin-20cm-Unbuckled-Cable-5-PCs-Pack-p-749.html

Software

* Arduino IDE http://arduino.cc/en/main/software
* nodejs http://nodejs.org/download/ or `sudo apt-get install nodejs && sudo npm install -g bower`


Setup
---

You can adapt the arduino input port, here follows a list of the pin used in the current code

Analog

* Temperature sensor - pin 1
* Rotary - pin 2

Digital

* Relay - pin 2
* LED - pin 6
* SerialLCD - pin 11
* Relay - pin 2


Installation
---

* Push the code to the arduino board with the IDE
* on linux (and possibly mac too) add your user name to dialout `sudo usermod -a -G dialout USERNAME`. Test with `cat /dev/ttyUSB0` (adapt the name to your tty, and set it accordingly in ./config/index.js)
* Setup the server code editing config/index.js and set your ip and port. Then

```
cd thermo-js

npm install 
bower install

node app.js

```


License
---
MIT

I'm not responsible for the use of the code in ANY WAY. YOU'LL USE IT AT YOUR OWN RISK.

