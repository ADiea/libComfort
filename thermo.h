#ifndef _LIB_THERMO_H_
#define _LIB_THERMO_H_

//Defines
#define DEW_ACCURATE 0
#define DEW_FAST 1
#define DEW_ACCURATE_FAST 2
#define DEW_FASTEST 3

//Gases
#define GAS_VOC 0 /* volatile organic compounts, NOx and other gases detected by MQ135 */
#define GAS_CO2 1 /* carbon dioxide */
#define GAS_CO 2 /* carbon monoxide */

typedef tempCelsius float; /**Temperature in degrees Celsius  */
typedef tempFahrenheit float; /**Temperature in degrees Fahrenheit  */
typedef relHumidity float; /** Relative Humidity [0.0, 100.0] */
typedef mbar float; /** Pressure expresse3d in millibar */
typedef ppm float; /** Gas concentration expressed in parts per milion*/

//Constants - see http://www.engineeringtoolbox.com/human-air-d_186.html
#define HUMAN_HEAT_PER_HOUR 118 //Watts. Between 19-34*C

#define HUMAN_AIR_BREATHE_AT_REST 0.5 //m3/hour
#define HUMAN_AIR_BREATHE_AT_LIGHT_WORK 1.5 //m3/hour
#define HUMAN_AIR_BREATHE_AT_HARD_WORK 3 //m3/hour

#define HUMAN_BREATH_OXYGEN 16.5 //percent
#define HUMAN_BREATH_CO2 4.0 //percent. the rest is nitrogen





/* *** *** *** *** *** *** *** *** *** USER SWITCHES *** *** *** *** *** *** *** *** *** *** *** *** */

/* Number of control points for the comfort profile */
#define COMFORT_CTRL_POINTS 5 

/* Used algorithm for determining the dew point */
#define DEW_ALGORITHM DEW_ACCURATE_FAST


//Other defines

//Utility functions
inline tempFahrenheit convertCtoF(tempCelsius c){ return c * 1.8f + 32; }
inline tempCelsius convertFtoC(tempFahrenheit f){ return (f-32)/1.8f; }



/* *** *** *** *** *** *** *** *** *** COMFORT *** *** *** *** *** *** *** *** *** *** *** *** */
enum ComfortState
{
	Comfort_OK = 0,
	Comfort_TooHot = 1,
	Comfort_TooCold = 2,
	Comfort_TooDry = 4,
	Comfort_TooHumid = 8,
	
	Comfort_HotAndDry = Comfort_TooHot | Comfort_TooDry,
	Comfort_ColdAndDry = Comfort_TooCold | Comfort_TooDry,
	Comfort_HotAndHumid = Comfort_TooHot | Comfort_TooHumid,	
	Comfort_ColdAndHumid = Comfort_TooCold | Comfort_TooHumid
};

//Temperature and humidity, a point on the 2D comfort diagram
struct ComfortPoint
{
	relHumidity x_humid;
	tempCelsius y_temp;
}

//A line joining 2 comfort points. Described by equation ax + by + c = 0
//Given 2 points A(x1,y1) B(x2,y2) here are the formulats to dete3rmine a b c
// a = y2-y1 ; b = x1-x2 ; c = x2*y1 - x1*y2
struct ComfortLine
{
	float a,b,c;
}

struct ComfortProfile
{
	//constructor
	ComfortProfile();
	
	//deserialize profile from stored string
	bool load(uint8_t* srcSerializedProfile, size_t len);
	
	//serialize profile to string
	bool save(uint8_t* dstSerializedProfile, size_t maxlen);

	//recalculate constants for all control lines
	void initComfortLines();

	//recalculate constants for lines connected 
	//to modified control point
	void refreshComfortLine(uint8_t idxModifiedPoint);

	//returns the average temperatura between the hot line 
	// and the cold line based on the current humidity
	tempCelsius getAverageProfileTemperature(relHumidity humidity);

	//this updates the current temperature and modifies 
	//the closest control point on the hot or cold line 
	//adjusting the user comfort profile
	void setUserTemperature(tempCelsius temp); 

	//called when temperature changed
	void onNewTemperature(tempCelsius newTemp);

	//called when humidity changed
	void onNewHumidity(relHumidity newHumid);
	
	//get simple information about the last sensor measurements
	bool isTooHot();
	bool isTooHumid();
	bool isTooCold();
	bool isTooDry();

	//compute the comfort state and the comfort percent based on
	//last sensor measurements
	void getComfort(ComfortState& state, float& percent);
	
	//return: the average between the hot line value and cold line value 
	//			coresponding to humidity
	void getComfortTemperature(relHumidity humidity);
	
	//// struct describing the current profile ////

	//comfort points defining the tooHot line and the associated lines
	ComfortPoint hotPoints[COMFORT_CTRL_POINTS];
	ComfortLine  hotLines[COMFORT_CTRL_POINTS - 1];

	//comfort points defining the tooCold line and the associated lines
	ComfortPoint coldPoints[COMFORT_CTRL_POINTS];
	ComfortLine  coldLines[COMFORT_CTRL_POINTS - 1];
	
	//the relative humidity values considered the dry and humid borders
	relHumidity dryLine;
	relHumidity humidLine;

	//latest temp and humidity from sensor
	ComfortPoint lastSensorMeasurements;	
	
	//user desired temperature
	//float currentUserSetTemperature;
}

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */


//The unit this lib uses for temperature is *Celsius (as read from sensors).
//If you call a function please ensure you pass the temperature in Celsius
//
//You can use the convertFtoC and convertCtoF functions to convert the parameters 
//and results if you require Fahrenheit units
class Comfort
{
public:
	Comfort();
	
	/** 
	brief: set the latest temperature and humidity read from sensor(s)
	*/
	void onNewTempAndHumidity(tempCelsius t, relHumidity h);

	/** 
	brief: set the latest pressure read from sensor(s)
	*/
	void onNewPressure(mbar p);
	
	/** 
	brief: compute dew point on last temperature and humidity set with onNewTempAndHumidity()
	return: the dew point in *C
	*/
	tempCelsius getDewPoint();
	
	/** 
	brief: compute the heat index as defined by US NOAA 
		   on last temperature and humidity set with onNewTempAndHumidity()
		   reference: http://www.srh.noaa.gov/ama/?n=heatindex
	return: the heat index in *C
	*/
	tempCelsius getHeatIndexUS();
	
	/** 
	brief: compute the heat index(humindex) as defined in Canada
		   on last temperature and humidity set with onNewTempAndHumidity()
		   reference: https://en.wikipedia.org/wiki/Humidex
	return: the heat index in *C
	*/
	tempCelsius getHumindexCanada();
	
	/** 
	brief: get the comfort profile to be able to save/load or query current comfort status
	return: return the comfort profile
	*/	
	ComfortProfile& getComfortProfile();

private:
	ComfortProfile m_curComfProfile;
	tempCelsius m_t;
	relHumidity m_h;

};

#endif

