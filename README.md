# libThermo
Thermodynamic and physiological constants and routines for determining human comfort and meteorological data based on temperature, pressure, RH, dust, CO and CO2 concentration.

This should prove useful for HVAC projects (thermostats, (de)humidifiers, ventilation), indoor and outdoor condition assessment, home automation, etc.

##Features

 1. Dew point [https://en.wikipedia.org/wiki/Dew_point]
 2. Heat index (US) [https://en.wikipedia.org/wiki/Heat_index]
 3. Humindex (Canada) [https://en.wikipedia.org/wiki/Humidex]
 4. Human physiology constants: needed air for breathing, generated heat, healthy limits for various gases  
 5. Thermal comfort - takes into account the relation between temperature and RH
	* Empiric comfort function based on comfort profiles(parametric lines)
	* Current comfort profile adapts as the user sets new temperature.	
	* Determine optimal temperature and if it's too cold, hot, humid, dry, based on current comfort profile
	* Multiple comfort profiles possible. Default based on http://epb.apogee.net/res/refcomf.asp	
 6. // Fresh air requirements [http://www.engineeringtoolbox.com/air-change-rate-d_882.html]	
 7. Temperature and Pressure conversion routines
 
Feed in temperature, pressure, relative humidity, gas & dust concentration data from your sensors.