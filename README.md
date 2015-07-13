# libComfort
Thermodynamic and physiological routines and constants for determining human comfort and meteorological data based on temperature, pressure, RH, dust, CO and CO2 concentration.

This should prove useful for HVAC projects (thermostats, (de)humidifiers, ventilation), indoor and outdoor condition assessment, home automation, etc.

##Features

 1. Dew point [https://en.wikipedia.org/wiki/Dew_point]
 2. Heat index (US) [https://en.wikipedia.org/wiki/Heat_index]
 3. Humindex (Canada) [https://en.wikipedia.org/wiki/Humidex]
 5. Thermal comfort - takes into account the relation between temperature and RH
	* Empiric comfort function based on comfort profiles(parametric lines)
	* Current comfort profile adapts as the user sets new temperature.	
	* Determine optimal temperature and if it's too cold, hot, humid, dry, based on current comfort profile
	* Multiple comfort profiles possible. Default based on http://epb.apogee.net/res/refcomf.asp	
 4. Human physiology data: healthy limits for RH and various gases, fresh air needs, generated human heat 	
 7. Temperature and Pressure conversion utility routines

##Credits

Based in part on *DHT-sensor-library* https://github.com/adafruit/DHT-sensor-library

Based in part on *DHTlib* https://github.com/RobTillaart/Arduino/tree/master/libraries/DHTlib

Based on information from https://en.wikipedia.org and http://www.engineeringtoolbox.com

Used *Sming Framework* for the ESP8266 platform https://github.com/anakod/Sming

## Disclaimer

**This was tested on an ESP8266 module(ESP-07) running at 80Mhz.**

**_This is in theory compatible with Arduino but untested yet_**

