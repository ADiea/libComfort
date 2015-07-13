#ifndef _LIB_THERMO_H_
#define _LIB_THERMO_H_

//algorithms for determining dew point 
#define DEW_ACCURATE 0
#define DEW_FAST 1
#define DEW_ACCURATE_FAST 2
#define DEW_FASTEST 3

#define PRESSURE_CONVERT_OFF 0	/*No pressure conversion function*/
#define PRESSURE_CONVERT_FAST 1	/*For ESP8266 arch: RAM: xx Bytes  Speed: xx ms*/
#define PRESSURE_CONVERT_SLIM 0 /*For ESP8266 arch: RAM: xx Bytes  Speed: xx ms*/

/* *** *** *** *** *** *** *** *** *** USER SWITCHES *** *** *** *** *** *** *** *** *** *** *** *** */

/* Number of control points for the comfort profile */
#define COMFORT_CTRL_POINTS 5 

/* Used algorithm for determining the dew point */
#define DEW_ALGORITHM DEW_ACCURATE_FAST

/* Used pressure conversion function */
#define PRESSURE_CONVERT_FCT PRESSURE_CONVERT_SLIM

/* *** *** *** *** *** *** *** *** *** GASES *** *** *** *** *** *** *** *** *** *** *** *** *** */

//Gases
#define GAS_VOC 0 /* volatile organic compounts, NOx and other gases detected by MQ135 */
#define GAS_CO2 1 /* carbon dioxide */
#define GAS_CO 2 /* carbon monoxide */

typedef tempCelsius float; /**Temperature in degrees Celsius  */
typedef tempFahrenheit float; /**Temperature in degrees Fahrenheit  */
typedef relHumidity float; /** Relative Humidity [0.0, 100.0] */
typedef mbar float; /** Pressure expresse3d in millibar */
typedef ppm float; /** Gas concentration expressed in parts per milion*/

//Constants see - http://www.engineeringtoolbox.com/human-air-d_186.html
//              - http://www.engineeringtoolbox.com/co2-persons-d_691.html
#define HUMAN_HEAT_PER_HOUR 118 //Watts. Between 19-34*C

#define HUMAN_AIR_BREATHE_AT_REST 0.5 //m3/hour
#define HUMAN_AIR_BREATHE_AT_LIGHT_WORK 2 //m3/hour
#define HUMAN_AIR_BREATHE_AT_HARD_WORK 6 //m3/hour

#define HUMAN_BREATHOUT_OXYGEN 0.165
#define HUMAN_BREATHOUT_CO2 0.04 //the rest is nitrogen

//Recommended low and high levels of RH for a healthy climate
//http://www.sterlingiaq.com/photos/1044922825.pdf
//RH values for health hazards:
//	Bacteria: RH < 30 && RH >60
//  Viruses: RH < 50 && RH >70
//  Fungi: RH >60
//  Respiratory infections: RH < 50
#define HEALTHY_RH_LOW 30
#define HEALTHY_RH_HIGH 60
#define HEALTHY_RH_THRESH 5

//Recommended summer indoor min/max temp vs outdoor temp relationship 
//based on liniarisation of tables from:
//http://www.engineeringtoolbox.com/indoor-design-temperatures-d_109.html
inline tempCelsius recomMinIndoorTempSummer(tempCelsius outDoorTemp)
{ return outDoorTemp * 0.357142f + 14.5f; }

inline tempCelsius recomMaxIndoorTempSummer(tempCelsius outDoorTemp)
{ return outDoorTemp * 0.035714f + 16.25f; }

//Recommended ppm gas levels for CO2 and CO
//http://www.engineeringtoolbox.com/co2-comfort-level-d_1024.html
//https://en.wikipedia.org/wiki/Carbon_monoxide_poisoning
struct GasLevel
{
	ppm maxValue;
	unsigned short maxExposureMinutes;
	char* description;
};

GasLevel CO2_Levels[] = 
{
	{400, 0, "OutdoorNormal"},
	{600, 0, "Acceptable"},
	{1000, 0, "HighLevel"},
	{2500, 0, "Drowsiness"},
	{5000, 0, "VeryHigh"},
	{30000, 0, "Nausea"},
	{50000, 0, "Headaches"},
	{100000, 0, "Death"},
}

char* get_CO2_ExposureDescription(ppm gasConcentration, unsigned short minutes);

//#define CO_INTOXICATION_SIMPTOMS "headache, tiredness, fatigue, nausea and dizziness"
GasLevel CO_Levels[] = 
{
	{9, 8*60, "Safe_8hour"},
	{26, 1*60, "Safe_1hour"},
	{52, 30, "Safe_30min"},
	{87, 15, "Safe_15min"},
	{200, 2*60, "LightHeadache"},
	{400, 1*60 "SeriousHeadache"},
	{800, 45, "DizzinessNausea"},
	{1600, 20, "DeathInOneHour"},
}

char* get_CO_ExposureDescription(ppm gasConcentration, unsigned short minutes);

/* *** *** *** *** *** *** *** *** *** PRESSURE *** *** *** *** *** *** *** *** *** *** *** *** *** */

enum PressureUnits
{
	unit_Pascal=0, /*Pascal*/
	unit_Bar,	/*Bar*/
	unit_At,		/*Technical atmosphere*/
	unit_Atm,	/*Standard atmosphere*/
	unit_Torr_mmHg,	/*Torr ~= 1 mmHg */
	unit_PSI,		/*Pounts per square inch*/
	unit_mBar,	/*milliBar*/
	unit_inHg, /*inch height of Mercury*/
	
	maxPressureUnits	/*Keep declared last*/
};

#if PRESSURE_CONVERT_FCT == PRESSURE_CONVERT_FAST
float pressureUnitConvertTable[maxPressureUnits]][maxPressureUnits] = 
{
/*Pascal		bar				at				atm				mmHg (torr)		PSI				mbar			inHg	*/
{1, 			1.0e-5, 		1.0197e-5, 		9.8692e-6, 		7.5006e-3, 		1.450377, 		1.0e-2, 		2.96e-4},
{1.0e+5, 		1, 				1.0197, 		0.98692, 		750.06, 		14.50377, 		1.0e+3, 		29.61},
{0.980665e+5, 	0.980665, 		1, 				0.967841, 		735.5592, 		14.22334, 		0.980665e+3, 	28.657772},
{1.01325e+5, 	1.01325, 		1.0332, 		1, 				760, 			14.69595, 		1.01325e+3, 	29.92},
{133.3224, 		1.333224e-3, 	1.359551e-3, 	1.315789e-3, 	1, 				1.933678e-2, 	1.333224, 		0.039},
{6.8948e+3, 	6.8948e-2, 		7.03069e-2, 	6.8046e-2, 		51.71493, 		1,  			68.948, 		2.04},
{1.0e+2, 		1.0e-3, 		1.0197e-3, 		0.98692e-3, 	750.06e-3, 		14.50377e-3, 	1, 				29.61e-3},
{3377, 			0.0338, 		0.034096, 		0.033, 			25.4, 			0.49, 			0.0338e+3,		1},
};

inline float convertPressure(float value, PressureUnits to, PressureUnits from)
{return value * pressureUnitConvertTable[to][from];}

#elif PRESSURE_CONVERT_FCT == PRESSURE_CONVERT_SLIM

float pressureUnitFromPascal[maxPressureUnits-1] = 
/*bar			at				atm				mmHg (torr)		PSI				mbar			inHg	*/
{1.0e-5, 		1.0197e-5, 		9.8692e-6, 		7.5006e-3, 		1.450377, 		1.0e-2, 		2.96e-4};

inline float convertPressure(float value, PressureUnits to, PressureUnits from)
{return value * pressureUnitFromPascal[to] / pressureUnitFromPascal[from];}

#endif

/* *** *** *** *** *** *** *** *** *** TEMPERATURE AND RH *** *** *** *** *** *** *** *** *** *** */

//Heatindex comfort map
//http://www.srh.noaa.gov/ama/?n=heatindex
struct HeatIndexLevel
{
	tempCelsius levelThreshold;
	char* description;
}

HeatIndexLevel heatIndexLevels = 
{
	{26.66, "Yellow_Fatigue"},
	{32.22, "Mustard_PossibleHeatStroke"},
	{39.44, "Orange_ProbableHeatStroke"},
	{51.11, "Red_LikelyHeatStroke"}
};

char* getHeatIndexDescription(tempCelsius t);

//Convert between temperature units
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

