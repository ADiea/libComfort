#ifndef _LIB_THERMO_H_
#define _LIB_THERMO_H_

//User switches
#define COMFORT_CTRL_POINTS 5

//Defines

enum ComfortState
{
	Comfort_OK = 0,
	Comfort_TooHot = 1,
	Comfort_TooCold = 2,
	Comfort_TooDry = 4,
	Comfort_TooHumid = 8,
	Comfort_HotAndHumid = 9,
	Comfort_HotAndDry = 5,
	Comfort_ColdAndHumid = 10,
	Comfort_ColdAndDry = 6
};

//Temperature and humidity, a point on the 2D comfort diagram
struct ComfortPoint
{
	float x_humid, y_temp;
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

	//recalculate constants for all control lines
	void initComfortLines();

	//recalculate constants for lines connected 
	//to modified control point
	void refreshComfortLine(uint8_t idxModifiedPoint);

	//returns the average temperatura between the hot line 
	// and the cold line based on the current humidity
	void getAverageProfileTemperature(float humidity);

	//this updates the current temperature and modifies 
	//the closest control point 
	void setTemperature(float temp); 

	//called when temperature changed
	void onTemperature(float newTemp);

	//called when humidity changed
	void onHumidity(float newHumid);
	
	bool isTooHot(float temp, float humidity);
	bool isTooHumid(float temp, float humidity);
	bool isTooCold(float temp, float humidity);
	bool isTooDry(float temp, float humidity);
	

	//// vars describing the current profile ////
	
	//comfort points defining the tooHot line and the associated lines
	ComfortPoint hotPoints[COMFORT_CTRL_POINTS];
	ComfortLine  hotLines[COMFORT_CTRL_POINTS - 1];

	//comfort points defining the tooCold line and the associated lines
	ComfortPoint coldPoints[COMFORT_CTRL_POINTS];
	ComfortLine  coldLines[COMFORT_CTRL_POINTS - 1];

	//latest temp and humidity from sensor
	ComfortPoint lastSensorMeasurements;

	//last computed comfort state and 
	ComfortState lastComfortState;
	float lastComfortPercent; 

	float currentSetTemperature;
}

class Thermo
{
public:

//Utils


private:


};

#endif

