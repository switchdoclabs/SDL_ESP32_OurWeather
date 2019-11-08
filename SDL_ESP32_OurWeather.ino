// Filename WeatherPlus.ino
// Version 050 September 2019
// SwitchDoc Labs, LLC
//

//
//


#define WEATHERPLUSESP32VERSION "050P-G"
#define CONTROLLERBOARD "V2"

#define WEATHERPLUSPUBNUBPROTOCOL "OURWEATHER050"

// define OWDEBUG to print out lots of debugging information for WeatherPlus.


#define OWDEBUG

#undef PUBNUB_DEBUG

#define BLINKPIN 13



#define DEFAULTCLOCKTIMEOFFSETTOUTC -25200

#define DEBUGBLYNK

#define BLYNK_NO_BUILTIN

#define BLYNK_PRINT Serial // Defines the object that is used for printing
#undef BLYNK_DEBUG
#define BLYNK_USE_128_VPINS

#include <BlynkSimpleEsp32.h>


// Change this to undef if you don't have the OLED present
#define OLED_Present

// BOF preprocessor bug prevent - insert on top of your arduino-code
#if 1
__asm volatile ("nop");
#endif

// Board options

#pragma GCC diagnostic ignored "-Wwrite-strings"
// Board options


/*
  extern "C" {
  #include "user_interface.h"
  }
*/

#if defined(ARDUINO) && ARDUINO >= 100
// No extras
#elif defined(ARDUINO) // pre-1.0
// No extras
#elif defined(ESP_PLATFORM)
#include "arduinoish.hpp"
#endif



//#include "Time/TimeLib.h"
#include "TimeLib.h"


#include <TimeLib.h>
#include "Clock.h"

bool WiFiPresent = false;

#include <WiFi.h>

#include <esp_wps.h>
#include <esp_smartconfig.h>


#define ESP_WPS_MODE WPS_TYPE_PBC

// Kludge for latest ESP32 SDK - July 1, 2018

#define WPS_CONFIG_INIT_DEFAULT(type) { \
    .wps_type = type, \
                .crypto_funcs = &g_wifi_default_wps_crypto_funcs, \
  }

esp_wps_config_t config = WPS_CONFIG_INIT_DEFAULT(ESP_WPS_MODE);



#include <Preferences.h>;

#include "NTPClient.h"

WiFiUDP ntpUDP;


NTPClient timeClient(ntpUDP);

/* create an instance of Preferences library */
Preferences preferences;


#include <WiFiClientSecure.h>
#include <HTTPClient.h>



//needed for library
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>



#include "WiFiManager.h"          //https://github.com/tzapu/WiFiManager

//gets called when WiFiManager enters configuration mode


void configModeCallback (WiFiManager *myWiFiManager)
//void configModeCallback ()
{

  Serial.println("Entered AP config mode");

  Serial.println(WiFi.softAPIP());

}

// OTA updated
#include <WiFiMulti.h>



#include <HTTPClient.h>
#include <HTTPUpdate.h>



#include "config.h"

int pubNubEnabled;

String SDL2PubNubCode  = "";
String SDL2PubNubCode_Sub = "";

// Blynk Codes
String BlynkAuthCode = "";
bool UseBlynk = false;


BlynkTimer Btimer;



// Attach virtual serial terminal to Virtual Pin
WidgetTerminal statusTerminal(V32);

#define PUBLISHINTERVALSECONDS 30


#define PubNub_BASE_CLIENT WiFiClient

#define PUBNUB_DEFINE_STRSPN_AND_STRNCASECMP

#include "PubNub.h"




// parsing function
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {
    0, -1
  };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}



//

char channel1[]  = "OWIOT1";
char uuid[]   = WEATHERPLUSPUBNUBPROTOCOL;








#include <Wire.h>

#include <Arduino.h> //needed for Serial.println

// debug the REST library

#define DEBUG_MODE 1

#include "MaREST.h"

#include <String.h>





// display modes
#define DISPLAY_POWERUP 0
#define DISPLAY_IPDISPLAY 1
#define DISPLAY_WEATHER_SMALL 2
#define DISPLAY_WEATHER_MEDIUM 3
#define DISPLAY_WEATHER_LARGE 4
#define DISPLAY_STATUS 5
#define DISPLAY_ACCESSPOINT 6
#define DISPLAY_WEATHER_DEMO 7
#define DISPLAY_TRYING_AP 8
#define DISPLAY_FAILING_AP 9
#define DISPLAY_DATETIME 10
#define DISPLAY_UPDATING 11
#define DISPLAY_NO_UPDATE_AVAILABLE 12
#define DISPLAY_NO_UPDATE_FAILED 13
#define DISPLAY_UPDATE_FINISHED 14
#define DISPLAY_SUNAIRPLUS 16
#define DISPLAY_WXLINK 17
#define DISPLAY_SDL2PUBNUBCODE 18
#define DISPLAY_FAILED_RECONNECT 19
#define DISPLAY_LIGHTNING_STATUS 20
#define DISPLAY_LIGHTNING_DISPLAY 21
#define DISPLAY_TRYING_SMARTCONFIG 22
#define DISPLAY_TRYING_WPS 24

#define DEBUG


// Rest Interface


#define PREFIX ""

String RestTimeStamp;
String RestDataString;
String Version;
String controllerBoard;





//----------------------------------------------------------------------
//Local WiFi


int WiFiSetupFlag = 0;

String APssid;

String Wssid;
String WPassword;

WiFiServer server(WEB_SERVER_PORT);

IPAddress myConnectedIp;
IPAddress myConnectedGateWay;
IPAddress myConnectedMask;


//----------------------------------------------------------------------


int blinkPin = BLINKPIN;                // pin to blink led at each reading
// Create an instance of the server


// Create aREST instance
aREST rest = aREST();
// commands are functions that get called by the webserver framework
// they can read any posted data from client, and they output to server




#include "elapsedMillis.h"

elapsedMillis timeElapsed; //declare global if you don't want it reset every time loop

elapsedMillis timeElapsed300Seconds; //declare global if you don't want it reset every time loop


//

#include "MAdafruit_BMP280.h"

Adafruit_BMP280 bme;


#define SENSORS_PRESSURE_SEALEVELHPA 1015.00
float altitude_meters;
float BMP280_Temperature;
float BMP280_Pressure;
float BMP280_Altitude;

bool BMP280Found;


int EnglishOrMetric;   // 0 = English units, 1 = Metric

int WeatherDisplayMode;



// SHT30

float SHT30_Temperature;
float SHT30_Humidity;
float dewpoint;


// SHT30
#include "WEMOS_SHT3X.h"

SHT3X sht30(0x44);
bool SHT30_Present = false;

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// TSL2591

#include "TSL2591.h"



bool TSL2591_Present = false;



TSL2591 TSL2591;

#include "AS3935.h"

double TSL2591_Lux = 0.0;

// ThunderBoard AS3935 from SwitchDoc Labs
AS3935 as3935(0x02, 3);


// lightning state variables as3935

String as3935_LastLightning = "";
int as3935_LastLightningDistance = 0;
String as3935_LastEvent = "";
int as3935_LastReturnIRQ = 0;
String as3935_LastLightningTimeStamp = "";
String as3935_LastEventTimeStamp = "";
int as3835_LightningCountSinceBootup = 0;

String as3935_FullString = "";
String as3935_Params = "";

int as3935_NoiseFloor = 2;

bool as3935_Indoor = true;

int as3935_TuneCap = 7;
bool as3935_DisturberDetection = false;
int as3935_WatchdogThreshold = 3;
int as3935_SpikeDetection = 3;

bool AS3935Present = false;

void printAS3935Registers()
{
  int noiseFloor = as3935.getNoiseFloor();
  int spikeRejection = as3935.getSpikeRejection();
  int watchdogThreshold = as3935.getWatchdogThreshold();
  Serial.print("Noise floor is: ");
  Serial.println(noiseFloor, DEC);
  Serial.print("Spike rejection is: ");
  Serial.println(spikeRejection, DEC);
  Serial.print("Watchdog threshold is: ");
  Serial.println(watchdogThreshold, DEC);
}

int parseOutAS3935Parameters()
{

  // check for bad string

  if (as3935_Params.indexOf(",") == -1)
    as3935_Params = "2,1,7,0,3,3";

  String Value;

  Value = getValue(as3935_Params, ',', 0);
  if ((Value.toInt() < 0) || (Value.toInt() > 7))
    return 2;


  Value = getValue(as3935_Params, ',', 1);
  if ((Value.toInt() < 0) || (Value.toInt() > 1))
    return 2;

  Value = getValue(as3935_Params, ',', 2);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;

  Value = getValue(as3935_Params, ',', 3);
  if ((Value.toInt() < 0) || (Value.toInt() > 1))
    return 2;

  Value = getValue(as3935_Params, ',', 4);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;

  Value = getValue(as3935_Params, ',', 5);
  if ((Value.toInt() < 0) || (Value.toInt() > 15))
    return 2;


  // OK, if we are here then all data is good
  Value = getValue(as3935_Params, ',', 0);
  as3935_NoiseFloor = Value.toInt();
  Value = getValue(as3935_Params, ',', 1);
  as3935_Indoor = Value.toInt();
  Value = getValue(as3935_Params, ',', 2);
  as3935_TuneCap = Value.toInt();
  Value = getValue(as3935_Params, ',', 3);
  as3935_DisturberDetection = Value.toInt();
  Value = getValue(as3935_Params, ',', 4);
  as3935_WatchdogThreshold = Value.toInt();
  Value = getValue(as3935_Params, ',', 5);
  as3935_SpikeDetection = Value.toInt();

  return 1;
}

void setAS3935Parameters()
{


  as3935.setTuningCapacitor(as3935_TuneCap);   // set to 1/2 - middle - you can calibrate on an Arduino UNO and use the value from there (pf/8)








  // lightning state variables as3935



  // first let's turn on disturber indication and print some register values from AS3935
  // tell AS3935 we are indoors, for outdoors use setOutdoors() function
  if (as3935_Indoor == true)
  {
    as3935.setIndoor();
  }
  else
  {
    as3935.setOutdoor();
  }

  as3935.setNoiseFloor(as3935_NoiseFloor);

#ifdef OWDEBUG
  Serial.print("NoiseFloor=");
  Serial.println(as3935_NoiseFloor);
#endif

  //AS3935.calibrate(); // can't calibrate because IRQ is polled and not through an Interrupt line on ESP8266

  // turn on indication of distrubers, once you have AS3935 all tuned, you can turn those off with disableDisturbers()

  if (as3935_DisturberDetection == true)
  {
    as3935.enableDisturbers();
  }
  else
  {
    as3935.disableDisturbers();
  }

  uint16_t getWatchdogThreshold(void);
  uint16_t setWatchdogThreshold(uint16_t wdth);


  as3935.setSpikeRejection(as3935_SpikeDetection);
  as3935.setWatchdogThreshold(as3935_WatchdogThreshold);

  // end set parameters


  // set up as3935 REST variable
  as3935_Params = String(as3935_NoiseFloor) + ",";
  as3935_Params += String(as3935_Indoor) + ",";
  as3935_Params += String(as3935_TuneCap) + ",";
  as3935_Params += String(as3935_DisturberDetection) + ",";
  as3935_Params += String(as3935_WatchdogThreshold) + ",";
  as3935_Params += String(as3935_SpikeDetection) ;


  printAS3935Registers();
}


// Station Name

String stationName;
String adminPassword;

// Health Indications for WeatherPlus

int heapSize;

// WeatherUnderground

String WeatherUnderground_StationID;
String WeatherUnderground_StationKey;
int lastMessageID;

// RTOS

#include "RTOSDefs.h"


// WeatherRack


float windSpeedMin;
float windSpeedMax;
float windGustMin;
float windGustMax;
float windDirectionMin;
float windDirectionMax;



float currentWindSpeed;
float currentWindGust;
float currentWindDirection;

float rainTotal;


float rainCalendarDay;
int lastDay;

float startOfDayRain;

#include "SDL_RasPiGraphLibrary.h"
// setup the RasPiConnect Graph Arrays
SDL_RasPiGraphLibrary windSpeedGraph(10, SDL_MODE_LABELS);
SDL_RasPiGraphLibrary windGustGraph(10, SDL_MODE_LABELS);
SDL_RasPiGraphLibrary windDirectionGraph(10, SDL_MODE_LABELS);


char windSpeedBuffer[150];  // wind speed graph
char windGustBuffer[150];  // wind speed graph
char windDirectionBuffer[150];  // wind speed graph





// WeatherRack

// LED connected to digital GPIO 0
int WpinLED = 0;
// Anenometer connected to GPIO 14
int pinAnem = 14;
// Rain Bucket connected to  GPIO 12
int pinRain = 12;

#include "OurWeatherPreferences.h";
#include "utility.h"

// include GET IP routines
#include "SDL_ESP32_BC24_GETIP.h"

#include "OWMAdafruit_ADS1015.h"


Adafruit_ADS1015 ads1015(0x49);

int current_quality = -1;
Adafruit_ADS1115 adsAirQuality(0x49);



long currentAirQuality;
long currentAirQualitySensor;
int INTcurrentAirQualitySensor;
bool AirQualityPresent = false;

#include "AirQualitySensor.h"

#include "SDL_Weather_80422.h"


//SDL_Weather_80422 weatherStation(pinAnem, pinRain, 0, 0, A0, SDL_MODE_INTERNAL_AD );
SDL_Weather_80422 weatherStation(pinAnem, pinRain, 0, 0, A0, SDL_MODE_I2C_ADS1015 );


// SDL_MODE_I2C_ADS1015
//

// RasPiConnect








long messageCount;

static uint8_t mac[] = LOCALMAC;
static uint8_t ip[] = LOCALIP;

// this is our current command object structure.  It is only valid inside void jsonCmd
typedef struct {
  char ObjectID[40];
  char ObjectFlags[40];
  char ObjectAction[40];
  char ObjectName[40];
  char ObjectServerID[40];
  char Password[40];
  char ObjectType[40];
  char Validate[40];
} currentObjectStructure;

char *md5str;

char ST1Text[40];   // used in ST-1 Send text control


char bubbleStatus[40];   // What to send to the Bubble status


#include "RainFunctions.h"

float lastRain;
#include "WeatherUnderground.h"



#include "WeatherUtils.h"





// OLED Constants


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// aREST functions

#include "aRestFunctions.h"


#include "SDL2PubNub.h"



// SunAirPlus

bool SunAirPlus_Present;

float BatteryVoltage;
float BatteryCurrent;
float LoadVoltage;
float LoadCurrent;
float SolarPanelVoltage;
float SolarPanelCurrent;

// WXLink Support


#include "Crc16.h"

//Crc 16 library (XModem)
Crc16 crc;

bool WXLink_Present;


float WXBatteryVoltage;
float WXBatteryCurrent;

float WXLoadCurrent;
float WXSolarPanelVoltage;
float WXSolarPanelCurrent;
long WXMessageID;
bool WXLastMessageGood;


#include "WXLink.h"





#include "SDL_Arduino_INA3221.h"

SDL_Arduino_INA3221 SunAirPlus;

// the three channels of the INA3221 named for SunAirPlus Solar Power Controller channels (www.switchdoc.com)
#define LIPO_BATTERY_CHANNEL 1
#define SOLAR_CELL_CHANNEL 2
#define OUTPUT_CHANNEL 3

// OLED Display

#include "OWMAdafruit_GFX.h"
#include "ESP_SSD1306.h"


#define min(a,b) ((a)<(b)?(a):(b))

#define OLED_RESET 4



ESP_SSD1306 display(OLED_RESET);

#include "OLEDDisplay.h"

#include "OLEDLoopDisplay.h"
//
//

//
//


// validate temperature from SHT30 - Fixes the rare +16 degrees C issue
bool invalidTemperatureFound;

float validateTemperature(float incomingTemperature)
{
  if (incomingTemperature > SHT30_Temperature + 15.0) // check for large jump in temperature
  {
    // OK, we may have an invalid temperature.  Make sure this is not a startup (current humidity will be 0.0 if startup)
    if (SHT30_Humidity < 0.1)
    {
      // we are in startup phase, so accept temperature
      invalidTemperatureFound = false;
      return incomingTemperature;
    }
    else
    {
      // we have an issue with a bad read (typically a +32 degrees C increase)
      // so send last good temperature back and flag a bad temperature
      invalidTemperatureFound = true;
      return SHT30_Temperature;
    }
  }
  if (incomingTemperature < SHT30_Temperature - 15.0) // check for large decrease in temperature
  {
    // OK, we may have an invalid temperature.  Make sure this is not a startup (current humidity will be 0.0 if startup)
    if (SHT30_Humidity < 0.1)
    {
      // we are in startup phase, so accept temperature
      invalidTemperatureFound = false;
      return incomingTemperature;
    }
    else
    {
      // we have an issue with a bad read (typically a +32 degrees C increase)
      // so send last good temperature back and flag a bad temperature
      invalidTemperatureFound = true;
      return SHT30_Temperature;
    }
  }
  invalidTemperatureFound = false;
  return incomingTemperature; // good temperature

}




time_t lastBoot;

#include "BlynkRoutines.h"


bool isDataStale()
{
  bool dataStale;
  dataStale =  false;
  if (WXLink_Present)
  {
    if (lastMessageID != WXMessageID)
    {
      dataStale = false;
      lastMessageID = WXMessageID;
      //writeToBlynkStatusTerminal("WXLink Recieved ID=" + String(WXMessageID));
    }
    else
    {
      dataStale = true;
    }
  }
  return dataStale;
}

// readSensor Task RTOS routine
void readSensors()
{










  tmElements_t tm;
  Serial.println("---------------");
  Serial.println("Internal Clock");
  Serial.println("---------------");



  String currentTimeString;
  currentTimeString = returnDateTime(now());


  Serial.println(currentTimeString);

  RestTimeStamp = currentTimeString;

  RestDataString = "";



  Serial.println("---------------");
  Serial.println();
  Serial.println("---------------");
  if (TSL2591_Present)
  {
    Serial.println("TSL2591");
    TSL2591_Lux = TSL2591.readIlluminance_TSL2591();
  }
  else
  {
    Serial.println("TSL2591 Not Present");
  }

  Serial.println("---------------");



  Serial.println();
  Serial.println("---------------");

  if (SHT30_Present)
  {
    Serial.println("SHT30");
  }
  else
  {
    Serial.println("SHT30 Not Present");
  }

  Serial.println("---------------");

  if (!WXLink_Present)
  {

    if (SHT30_Present)
    {

      int sht30_success;
      sht30_success = sht30.get();

      if (sht30_success == 0)
      {

        Serial.println("SHT30 Found");
        Serial.print("SHT30Temp=");
        Serial.println(sht30.cTemp);
        Serial.print("SHT30Humid=");
        Serial.println(sht30.humidity);

        // Now set the old SHT30 variables

        SHT30_Temperature = sht30.cTemp;
        SHT30_Humidity = sht30.humidity;
        dewpoint =  SHT30_Temperature - ((100.0 - SHT30_Humidity) / 5.0);
      }


    }
  }
  else
  {
    Serial.println("WXLink Present - SHT30 local read overruled");
  }


  RestDataString += String(SHT30_Temperature, 2) + ",";
  RestDataString += String(SHT30_Humidity, 2) + ",";

  Serial.println("---------------");
  if (BMP280Found)
    Serial.println("BMP280");
  else
    Serial.println("No BMP280 Found");
  Serial.println("---------------");





  if (BMP280Found)
  {

    /* Display the results (barometric pressure is measure in hPa) */
    //BMP280_Pressure = bmp.readPressure();
    // Put Alitude in Meters

    BMP280_Pressure = bme.readSealevelPressure(altitude_meters);

    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(BMP280_Pressure / 100.0);
    Serial.println(" hPa");



    /* Calculating altitude with reasonable accuracy requires pressure    *
       sea level pressure for your position at the moment the data is
       converted, as well as the ambient temperature in degress
       celcius.  If you don't have these values, a 'generic' value of
       1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA
       in sensors.h), but this isn't ideal and will give variable
       results from one day to the next.
     *                                                                    *
       You can usually find the current SLP value by looking at weather
       websites or from environmental information centers near any major
       airport.
     *                                                                    *
       For example, for Paris, France you can check the current mean
       pressure and sea level at: http://bit.ly/16Au8ol                   */

    /* First we get the current temperature from the BMP085 */
    float temperature;
    temperature = bme.readTemperature();
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");


    BMP280_Temperature = temperature;




    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    float altitude;
    altitude = bme.readAltitude(SENSORS_PRESSURE_SEALEVELHPA);
    Serial.print("Altitude:    ");
    Serial.print(altitude);

    BMP280_Altitude = altitude;
    Serial.println(" m");

  }

  RestDataString += String(BMP280_Temperature, 2) + ",";
  RestDataString += String(BMP280_Pressure, 2) + ",";
  RestDataString += String(BMP280_Altitude, 2) + ",";



  if (AirQualityPresent)
  {

    Serial.println("---------------");
    Serial.println("AirQualitySensor");
    Serial.println("---------------");


#ifdef OWDEBUG

    int16_t ad0 = adsAirQuality.readADC_SingleEnded(0);
    int16_t ad1 = adsAirQuality.readADC_SingleEnded(1);
    int16_t ad2 = adsAirQuality.readADC_SingleEnded(2);
    int16_t ad3 = adsAirQuality.readADC_SingleEnded(3);

    Serial.print("ad0=");
    Serial.println(ad0);
    Serial.print("ad1=");
    Serial.println(ad1);
    Serial.print("ad2=");
    Serial.println(ad2);
    Serial.print("ad3=");

    Serial.println(ad3);



    int16_t ad0_0x49 = ads1015.readADC_SingleEnded(0);
    int16_t ad1_0x49 = ads1015.readADC_SingleEnded(1);
    int16_t ad2_0x49 = ads1015.readADC_SingleEnded(2);
    int16_t ad3_0x49 = ads1015.readADC_SingleEnded(3);

    Serial.print("ad0_0x49=");
    Serial.println(ad0_0x49);
    Serial.print("ad1_0x49=");
    Serial.println(ad1_0x49);
    Serial.print("ad2_0x49=");
    Serial.println(ad2_0x49);
    Serial.print("ad3_0x49=");
    Serial.println(ad3_0x49);
#endif

    currentAirQuality = getAirQuality();

  }

  Serial.println("---------------");
  if (SunAirPlus_Present)
    Serial.println("SunAirPlus");
  else
    Serial.println("SunAirPlus Not Present");
  Serial.println("---------------");

  // if SunAirPlus present, read charge data

  if (SunAirPlus_Present)
  {

    LoadVoltage = SunAirPlus.getBusVoltage_V(OUTPUT_CHANNEL);
    LoadCurrent = SunAirPlus.getCurrent_mA(OUTPUT_CHANNEL);


    BatteryVoltage = SunAirPlus.getBusVoltage_V(LIPO_BATTERY_CHANNEL);
    BatteryCurrent = SunAirPlus.getCurrent_mA(LIPO_BATTERY_CHANNEL);

    SolarPanelVoltage = SunAirPlus.getBusVoltage_V(SOLAR_CELL_CHANNEL);
    SolarPanelCurrent = -SunAirPlus.getCurrent_mA(SOLAR_CELL_CHANNEL);

#ifdef OWDEBUG
    Serial.println("");
    Serial.print("LIPO_Battery Load Voltage:  "); Serial.print(BatteryVoltage); Serial.println(" V");
    Serial.print("LIPO_Battery Current:       "); Serial.print(BatteryCurrent); Serial.println(" mA");
    Serial.println("");

    Serial.print("Solar Panel Voltage:   "); Serial.print(SolarPanelVoltage); Serial.println(" V");
    Serial.print("Solar Panel Current:   "); Serial.print(SolarPanelCurrent); Serial.println(" mA");
    Serial.println("");

    Serial.print("Load Voltage:   "); Serial.print(LoadVoltage); Serial.println(" V");
    Serial.print("Load Current:   "); Serial.print(LoadCurrent); Serial.println(" mA");
    Serial.println("");
#endif

  }
  else
  {

    LoadVoltage = 0.0;
    LoadCurrent = 0.0;


    BatteryVoltage = 0.0;
    BatteryCurrent = 0.0;

    SolarPanelVoltage = 0.0;
    SolarPanelCurrent = 0.0;


  }





  Serial.println("---------------");
  Serial.println("WeatherRack");
  Serial.println("---------------");

  if (WXLink_Present == false)
  {

    currentWindSpeed = weatherStation.current_wind_speed();
    currentWindGust = weatherStation.get_wind_gust();

    currentWindDirection = weatherStation.current_wind_direction();

    float oldRain = rainTotal;
    rainTotal = rainTotal + weatherStation.get_current_rain_total();
    if (oldRain < rainTotal)
    {
      strcpy(bubbleStatus, "It is Raining\0");
      writeToBlynkStatusTerminal((String)"It is Raining");
    }

    windSpeedGraph.add_value(currentWindSpeed);
    windGustGraph.add_value(currentWindGust);
    windDirectionGraph.add_value(currentWindDirection);

    windSpeedGraph.getRasPiString(windSpeedBuffer, windSpeedBuffer);
    windGustGraph.getRasPiString(windGustBuffer, windGustBuffer);
    windDirectionGraph.getRasPiString(windDirectionBuffer, windDirectionBuffer);

    windSpeedMin = windSpeedGraph.returnMinValue();
    windSpeedMax = windSpeedGraph.returnMaxValue();
    windGustMin = windGustGraph.returnMinValue();
    windGustMax = windGustGraph.returnMaxValue();
    windDirectionMin = windDirectionGraph.returnMinValue();
    windDirectionMax = windDirectionGraph.returnMaxValue();
  }
  else
  {
    // WXLink is PRESENT, take from WXLink

    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>");
    Serial.print("WXLastMessageGood =");
    Serial.println(WXLastMessageGood);
    Serial.println(">>>>>>>>>>>>>>>>>>>>>>>>>>>");

    if (WXLastMessageGood == true)  // if bad WX Message, don't change
    {

      currentWindSpeed = convert4BytesToFloat(buffer, 9);
      currentWindGust = convert4BytesToFloat(buffer, 21);

      currentWindDirection = convert2BytesToInt(buffer, 7);

      float oldRain = rainTotal;
      rainTotal = convert4BytesToLong(buffer, 17);

      if (oldRain < rainTotal)
      {
        strcpy(bubbleStatus, "It is Raining\0");
      }

      windSpeedGraph.add_value(currentWindSpeed);
      windGustGraph.add_value(currentWindGust);
      windDirectionGraph.add_value(currentWindDirection);

      windSpeedGraph.getRasPiString(windSpeedBuffer, windSpeedBuffer);
      windGustGraph.getRasPiString(windGustBuffer, windGustBuffer);
      windDirectionGraph.getRasPiString(windDirectionBuffer, windDirectionBuffer);

      windSpeedMin = windSpeedGraph.returnMinValue();
      windSpeedMax = windSpeedGraph.returnMaxValue();
      windGustMin = windGustGraph.returnMinValue();
      windGustMax = windGustGraph.returnMaxValue();
      windDirectionMin = windDirectionGraph.returnMinValue();
      windDirectionMax = windDirectionGraph.returnMaxValue();

      // Now overwrite outside temp/humidity

      SHT30_Temperature = validateTemperature(convert4BytesToFloat(buffer, 25));
      SHT30_Humidity = convert4BytesToFloat(buffer, 29);

      // calculate dewpoint
      dewpoint =  SHT30_Temperature - ((100.0 - SHT30_Humidity) / 5.0);


      // set up solar status and message ID for screen

      // if WXLINK present, read charge data



      WXLoadCurrent = convert4BytesToFloat(buffer, 41);


      WXBatteryVoltage = convert4BytesToFloat(buffer, 33);
      WXBatteryCurrent = convert4BytesToFloat(buffer, 37);

      WXSolarPanelVoltage = convert4BytesToFloat(buffer, 45);
      WXSolarPanelCurrent = convert4BytesToFloat(buffer, 49);

      WXMessageID = convert4BytesToLong(buffer, 57);

      /*   Serial.println("");
         Serial.print("WXLIPO_Battery Load Voltage:  "); Serial.print(WXBatteryVoltage); Serial.println(" V");
         Serial.print("WXLIPO_Battery Current:       "); Serial.print(WXBatteryCurrent); Serial.println(" mA");
         Serial.println("");

         Serial.print("WXSolar Panel Voltage:   "); Serial.print(WXSolarPanelVoltage); Serial.println(" V");
         Serial.print("WXSolar Panel Current:   "); Serial.print(WXSolarPanelCurrent); Serial.println(" mA");
         Serial.println("");

         Serial.print("WXLoad Current:   "); Serial.print(WXLoadCurrent); Serial.println(" mA");
         Serial.println("");
      */




    }




  }







  Serial.print("windSpeedMin =");
  Serial.print(windSpeedMin);
  Serial.print(" windSpeedMax =");
  Serial.println(windSpeedMax);
#ifdef OWDEBUG
  Serial.print("windSpeedBuffer=");
  Serial.println(windSpeedBuffer);
#endif
  Serial.print("windGustMin =");
  Serial.print(windGustMin);
  Serial.print(" windGustMax =");
  Serial.println(windGustMax);
#ifdef OWDEBUG
  Serial.print("windGustBuffer=");
  Serial.println(windGustBuffer);
#endif
  Serial.print("windDirectionMin =");
  Serial.print(windDirectionMin);
  Serial.print(" windDirectionMax =");
  Serial.println(windDirectionMax);

#ifdef OWDEBUG
  Serial.print("windDirectionBuffer=");
  Serial.println(windDirectionBuffer);
#endif
  Serial.print("currentWindSpeed=");
  Serial.print(currentWindSpeed);

  Serial.print(" \tcurrentWindGust=");
  Serial.print (currentWindGust);

  Serial.print(" \tWind Direction=");
  Serial.print(currentWindDirection);


  Serial.print(" \t\tCumulative Rain = ");
  Serial.println(rainTotal);


  Serial.println(" ");



  RestDataString += String(currentWindSpeed, 2) + ",";
  RestDataString += String(currentWindGust, 2) + ",";
  RestDataString += String(currentWindDirection, 2) + ",";
  RestDataString += String(rainTotal, 2) + ",";
  RestDataString += String(windSpeedMin, 2) + ",";
  RestDataString += String(windSpeedMax, 2) + ",";
  RestDataString += String(windGustMin, 2) + ",";
  RestDataString += String(windGustMax, 2) + ",";

  RestDataString += String(windDirectionMin, 2) + ",";
  RestDataString += String(windDirectionMax, 2) + ",";
  RestDataString += String(EnglishOrMetric) + "," ;
  RestDataString += currentTimeString + "," ;
  RestDataString += stationName + ",";
  RestDataString += String(currentAirQualitySensor) + ",";
  RestDataString += String(currentAirQuality) + ",";

  RestDataString += String(BatteryVoltage, 2) + ",";
  RestDataString += String(BatteryCurrent, 2) + ",";
  RestDataString += String(SolarPanelVoltage, 2) + ",";
  RestDataString += String(SolarPanelCurrent, 2) + ",";
  RestDataString += String(LoadVoltage, 2) + ",";
  RestDataString += String(LoadCurrent, 2) + ",";


  RestDataString += String(WXBatteryVoltage, 2) + ",";
  RestDataString += String(WXBatteryCurrent, 2) + ",";
  RestDataString += String(WXSolarPanelVoltage, 2) + ",";
  RestDataString += String(WXSolarPanelCurrent, 2) + ",";
  RestDataString += "0.00,";
  RestDataString += String(WXLoadCurrent, 2) + ",";

  if (invalidTemperatureFound == true)
  {
    RestDataString += "IVF:,";
  }
  else
  {
    RestDataString += "V:,";
  }
  invalidTemperatureFound = false;


  // Restart WiFi in case of connected, then lost connection
  if (WiFiPresent == true)
  {
    if (WiFi.status()  != WL_CONNECTED)

    {

    }
  }


  if (WXLastMessageGood == true)
  {
    RestDataString += "WXLMG ,";
  }
  else
  {
    RestDataString += "WXLMB ,";
  }

  RestDataString += String(pubNubEnabled) + ",";


  if (AS3935Present == true)
  {
    // Now check for Lightning ThunderBoard AS3935

    Serial.println("---------------");
    Serial.println("ThunderBoard AS3935 Lightning Detector");
    Serial.println("---------------");

    // first step is to find out what caused interrupt
    int strokeDistance = 0.0;
    int irqSource = 0;

    irqSource = as3935.getInterruptReason();
    Serial.print("as3935 irqSource: ");
    Serial.println(irqSource, HEX);

    Serial.print("as3935 irqSource: ");
    Serial.println(irqSource, BIN);




    if (irqSource > 0)
    {

      printAS3935Registers();
      as3935_LastReturnIRQ = irqSource;
      // returned value is bitmap field, bit 0 - noise level too high, bit 2 - disturber detected, and finally bit 3 - lightning!
      if (irqSource & 0b0001)
      {
        Serial.println("INT_NH Interrupt: Noise level too high, try adjusting noise floor");
        writeToBlynkStatusTerminal("ThunderBoard-Noise level too high");

        as3935_LastEvent = "Noise Level too high";

        as3935_LastEventTimeStamp = returnDateTime(now());
      }
      if (irqSource & 0b0100)
      {
        Serial.println("INT_D Interrupt: Disturber detected");
        writeToBlynkStatusTerminal("ThunderBoard-Disturber detected");
        as3935_LastEvent = "Disturber detected";

        as3935_LastEventTimeStamp = returnDateTime(now());
      }
      if (irqSource & 0b1000)
      {
        // need to find how far that lightning stroke, function returns approximate distance in kilometers,
        // where value 1 represents storm in detector's near victinity, and 63 - very distant, out of range stroke
        // everything in between is just distance in kilometers

        strokeDistance = as3935.getDistance();

        as3935_LastEvent = "Lightning detected";

        as3935_LastEventTimeStamp = returnDateTime(now());;

        as3935_LastLightning  = String(strokeDistance) + " km" ;
        as3935_LastLightningTimeStamp = returnDateTime(now());;
        as3935_LastLightningDistance = strokeDistance;
        as3835_LightningCountSinceBootup++;


        Serial.print("INT_L Interrupt: Lightning Detected.  Stroke Distance:");
        Serial.print(strokeDistance);
        Serial.println(" km");
        writeToBlynkStatusTerminal((String)"Lightning! Distance=" + String(strokeDistance) + (String)"km");
        writeToStatusLine((String)"Lightning! Distance=" + String(strokeDistance) + (String)"km");

        if (strokeDistance == 1)
          Serial.println("Storm overhead");
        if (strokeDistance == 63)
          Serial.println("Out of range lightning detected.");


        vTaskDelay(3000 / portTICK_PERIOD_MS);
        updateDisplay(DISPLAY_LIGHTNING_DISPLAY);
        vTaskDelay(3000 / portTICK_PERIOD_MS);




      }
    }

  }
  //  Lightning REST variable
  as3935_FullString = "";
  as3935_FullString += as3935_LastLightning + ",";
  as3935_FullString += as3935_LastLightningTimeStamp + ",";
  as3935_FullString += String(as3935_LastLightningDistance) + ",";
  as3935_FullString += as3935_LastEvent + ",";
  as3935_FullString += as3935_LastEventTimeStamp + ",";
  as3935_FullString += String(as3835_LightningCountSinceBootup);


  // Lighting Rest
  RestDataString += as3935_LastLightning + ",";
  RestDataString += as3935_LastLightningTimeStamp + ",";
  RestDataString += String(as3935_LastLightningDistance) + ",";
  RestDataString += as3935_LastEvent + ",";
  RestDataString += as3935_LastEventTimeStamp + ",";
  RestDataString += String(as3835_LightningCountSinceBootup);


  if (timeElapsed300Seconds > 300000)   // 5 minutes
  {


    String lastBootTimeString;
    lastBootTimeString = returnDateTime(lastBoot);

    Serial.print("lastBoot = ");
    Serial.println(lastBootTimeString);

    timeElapsed300Seconds = 0;

    // update rain


    add60MinuteRainReading(rainTotal - lastRain);
    lastRain = rainTotal;


    struct tm *myTMNow;
    time_t myNow;
    myNow = now();
    myTMNow = localtime(&myNow);



    if (myTMNow->tm_mday == lastDay)
    {
      rainCalendarDay = rainTotal - startOfDayRain;

    }
    else
    {
      lastDay = myTMNow->tm_mday;
      rainCalendarDay = 0.0;
      startOfDayRain = rainTotal;
    }




    bool dataStale;

    // check for stale data from WXLink


    dataStale = isDataStale();


    if ((WeatherUnderground_StationID.length() == 0) || (WeatherUnderground_StationKey.length() == 0) )
    {
      Serial.println("-----------");
      Serial.println(" WeatherUnderground Disabled");
      Serial.println("-----------");
    }
    else // Check next and the go for it.
      if ((WeatherUnderground_StationID == "XXXX") || (WeatherUnderground_StationKey == "YYYY") )
      {
        Serial.println("-----------");
        Serial.println(" WeatherUnderground Disabled");
        Serial.println("-----------");

      }
      else
      {
        Serial.println("Attempting to send data to WeatherUnderground");
        if (dataStale == false)
          Serial.println("WeatherUnderground Data New - sent");
        else
          Serial.println("WeatherUnderground Data Stale - Not sent");

        if (dataStale == false)
        {
          if (sendWeatherUndergroundData() == 0)
          {
            // Failed - try again
            sendWeatherUndergroundData();
          }


        }
      }

    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // send data up to PubNub

    if (pubNubEnabled == 1)
    {

      String SendString = "{\"FullDataString\": \"" + RestDataString + "\"}"; //Send the request

      // publish it

      publishPubNubMessage(SendString);
    }
  }


  updateDisplay(WeatherDisplayMode);


  if (AS3935Present == true)
  {
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    updateDisplay(DISPLAY_LIGHTNING_STATUS);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }

  if (SunAirPlus_Present)
  {

    vTaskDelay(3000 / portTICK_PERIOD_MS);
    updateDisplay(DISPLAY_SUNAIRPLUS);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }

  if (WXLink_Present)
  {

    vTaskDelay(3000 / portTICK_PERIOD_MS);
    updateDisplay(DISPLAY_WXLINK);
    vTaskDelay(3000 / portTICK_PERIOD_MS);

  }


  Serial.println("OutOfDisplay");
  // writeToBlynkStatusTerminal("Sensors Read");



}   // end of readSensors



void readWXLinkSensors()
{
  Serial.println("---------------");
  if (WXLink_Present)
    Serial.println("WXLink");
  else
    Serial.println("WXLink Not Present");
  Serial.println("---------------");

  // read variables from WXLink

  if (WXLink_Present)
  {
    if (readWXLink() == true)
    {
      WXLastMessageGood = true;
      blinkLED(2, 200);  // blink 2 for good message


    }
    else
    {

      WXLastMessageGood = false;



    }
  }

}

// RTOS

#include "RTOSTasks.h"

void setup() {


  invalidTemperatureFound = false;

  // WiFi reset loop fix - erase the WiFi saved area

  WiFi.persistent(false);




  BMP280Found = false;
  stationName = "";



  WeatherUnderground_StationID = "XXXX";
  WeatherUnderground_StationKey = "YYYY";

  adminPassword = "admin";
  altitude_meters = 637.0;  // default to 611

  pinMode(blinkPin, OUTPUT);        // pin that will blink every reading
  digitalWrite(blinkPin, LOW);


  Serial.begin(115200);           // set up Serial library at 9600 bps

  Serial.println();
  Serial.println();
  Serial.println("--------------------");
  Serial.println("OurWeather");
  Serial.println("--------------------");
  Serial.print("Version: ");
  Serial.println(WEATHERPLUSESP32VERSION);

  Serial.print("Compiled at:");
  Serial.print (__TIME__);
  Serial.print(" ");
  Serial.println(__DATE__);

  Serial.println("--------");


  lastBoot = now();
  struct tm *lastBootTm;

  lastBootTm = localtime(&lastBoot);

  rainCalendarDay = 0.0;
  startOfDayRain = 0.0;

  lastDay = lastBootTm->tm_mday;

  String currentTimeString;
  currentTimeString = returnDateTime(now());

  Serial.print("now fromRTC =");
  Serial.println(currentTimeString);


  timeElapsed = 0;

  Serial.print("Free heap on ESP32:");
  heapSize = ESP.getFreeHeap();
  Serial.println(heapSize, DEC);


  Serial.print("CPU0 reset reason: ");
  print_reset_reason(rtc_get_reset_reason(0));

  Serial.print("CPU1 reset reason: ");
  print_reset_reason(rtc_get_reset_reason(1));

  // RTOS


  xSemaphoreRESTCommand = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreRESTCommand);   // initialize
  //xSemaphoreTake( xSemaphoreRESTCommand, 10);   // start with this off

  xSemaphoreReadSensor = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreReadSensor);   // initialize
  xSemaphoreTake( xSemaphoreReadSensor, 10);   // start with this off

  xSemaphoreReadWXLink = xSemaphoreCreateBinary();
  xSemaphoreGive( xSemaphoreReadWXLink);   // initialize
  xSemaphoreTake( xSemaphoreReadWXLink, 10);   // start with this off


  Serial.println("RTOS Tasks Starting");

  xTaskCreatePinnedToCore(
    taskReadSensors,          /* Task function. */
    "taskReadSensors",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    2,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core



  xTaskCreatePinnedToCore(
    taskReadWXLink,          /* Task function. */
    "taskReadWXLink",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    3,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core



  xTaskCreatePinnedToCore(
    taskRESTCommand,          /* Task function. */
    "TaskRESTCommand",        /* String with name of task. */
    10000,            /* Stack size in words. */
    NULL,             /* Parameter passed as input of the task */
    3,                /* Priority of the task. */
    NULL,             /* Task handle. */
    1);               // Specific Core



#ifdef OLED_Present
  OLEDDisplaySetup();
  updateDisplay(DISPLAY_POWERUP);
#endif

  delay(2000);

  pinMode(32, INPUT_PULLUP);
  Serial.print("digital Read 32 = ");
  Serial.println(digitalRead(32));

  if (digitalRead(32) == 0)
  {
    Serial.println("GPIO32 button down - Invalidating Preferences");
    resetPreferences();

  }


  readPreferences();


  // now set up thunderboard AS3935



  // reset all internal register values to defaults
  as3935.reset();


  int noiseFloor = as3935.getNoiseFloor();

  Serial.print("noiseFloor=");
  Serial.println(noiseFloor);

  if (noiseFloor == 2)
  {
    Serial.println("AS3935 Present");
    AS3935Present = true;
  }
  else
  {
    Serial.println("AS3935 Not Present");
    AS3935Present = false;
  }

  if (AS3935Present == true)
  {

    parseOutAS3935Parameters();
    setAS3935Parameters();



  }


  //---------------------
  // Setup WiFi Interface
  //---------------------
  // 5 cases:
  // 1) Use stored SSID/psk
  // 2) SmartConfig received
  // 3) WPS Button
  // 4) Local AP
  // 5) Wifi Fails


  WiFiPresent = false;
  WiFi.persistent(false);
  WiFi.begin();


  // check for SSID

  if (WiFi_SSID.length() != 0)
  {
    // use existing SSID
    Serial.println("Using existing SSID/psk");

    Serial.printf("SSID="); Serial.println(WiFi_SSID);
    Serial.printf("psk="); Serial.println(WiFi_psk);
    WiFi.begin(WiFi_SSID.c_str(), WiFi_psk.c_str());
    //Wait for WiFi to connect to AP
    Serial.println("Waiting for Saved WiFi");
#define WAITFORCONNECT 15
    for (int i = 0; i < WAITFORCONNECT * 2; i++)
    {
      if (WiFi.status() == WL_CONNECTED)
      {

        Serial.println("");
        Serial.println("WiFI Connected.");

        Serial.printf("SSID="); Serial.println(WiFi_SSID);
        Serial.printf("psk="); Serial.println(WiFi_psk);
        WiFiPresent = true;

        // BC24ThreeBlink(Green, 1000);

        blinkLED(4, 300);  // blink four OK!

        break;
      }

      Serial.print(".");
      WiFiPresent = false;
      blinkLED(5, 300);  // blink five - fails!
      //BC24ThreeBlink(White, 1000);
    }
    Serial.println();

  }

  /*
    if (WiFiPresent == false)
    {
      // do SmartConfig
    #define WAITFORSTART 15
    #define WAITFORCONNECT 20
      updateDisplay(DISPLAY_TRYING_SMARTCONFIG);

      WiFiPresent = SmartConfigGetIP(WAITFORSTART, WAITFORCONNECT);

    }

    if (WiFiPresent != true)
    {
    #define WPSTIMEOUTSECONDS 30
      // now try WPS Button
      updateDisplay(DISPLAY_TRYING_WPS);
      WiFiPresent = WPSGetIP(WPSTIMEOUTSECONDS);

    }
  */

  if (WiFiPresent != true)
  {
#define APTIMEOUTSECONDS 60
    updateDisplay(DISPLAY_TRYING_AP);
    WiFiPresent = localAPGetIP(APTIMEOUTSECONDS);
  }


  if (WiFiPresent == true)
  {

    WiFi_SSID = WiFi.SSID();
    WiFi_psk = WiFi.psk();
  }
  // write out preferences

  writePreferences();
  if (WiFiPresent == true)
  {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    Serial.print("WiFi Channel= ");
    Serial.println(WiFi.channel());
  }

  //---------------------
  // End Setup WiFi Interface
  //---------------------


  // Now do NTP (set's time to 00:00:00 if not present)  January 1, 1970


  // changed later with setTimeOffset() ). Additionaly you can specify the
  // update interval (in milliseconds, can be changed using setUpdateInterval() ).
  timeClient.begin();
  timeClient.forceUpdate();

  timeClient.setTimeOffset(ClockTimeOffsetToUTC);
  if (WiFiPresent == true)
  {
    timeClient.setUpdateInterval(3600000);
    timeClient.update();
  }
  time_t t;
  if (WiFiPresent == true)
  {
    Serial.print("Getting epochTime = ");
    t = timeClient.getEpochTime();
    Serial.println(t);
  }
  else
  {
    Serial.println("No WiFi Present - NTP not set");
    t = 1;
  }



  setTime(t);


  digitalClockDisplay();
  if (WiFi.status()  == WL_CONNECTED)

    WiFiPresent = true;


  writePreferences();

  Serial.print("WiFi Channel= ");
  Serial.println(WiFi.channel());

  blinkLED(2, 300);  // blink twice - OK!
  heapSize = ESP.getFreeHeap();

  RestTimeStamp = "";
  RestDataString = "";
  Version =   WEATHERPLUSESP32VERSION;


  controllerBoard = CONTROLLERBOARD;
  server.begin();

  rest.variable("OurWeatherTime", &RestTimeStamp);
  rest.variable("FullDataString", &RestDataString);
  rest.variable("FirmwareVersion", &Version);

  rest.variable("IndoorTemperature", &BMP280_Temperature);
  rest.variable("BarometricPressure", &BMP280_Pressure);
  rest.variable("Altitude", &BMP280_Altitude);

  rest.variable("OutdoorTemperature", &SHT30_Temperature);
  rest.variable("OutdoorHumidity", &SHT30_Humidity);

  rest.variable("CurrentWindSpeed", &currentWindSpeed);
  rest.variable("CurrentWindGust", &currentWindGust);

  rest.variable("CurrentWindDirection", &currentWindDirection);
  rest.variable("Sunlight", &TSL2591_Lux);
  rest.variable("EnglishOrMetric", &EnglishOrMetric);

  rest.variable("RainTotal", &rainTotal);
  rest.variable("WindSpeedMin", &windSpeedMin);
  rest.variable("WindSpeedMax", &windSpeedMax);
  rest.variable("WindGustMin", &windGustMin);
  rest.variable("WindGustMax", &windGustMax);
  rest.variable("WindDirectionMin", &windDirectionMin);
  rest.variable("WindDirectionMax", &windDirectionMax);
  rest.variable("AirQualitySensor", &INTcurrentAirQualitySensor);

  // as3935 rest variables


  rest.variable("ThunderBoardLast", &as3935_FullString);
  rest.variable("ThunderBoardParams", &as3935_Params);

  rest.variable("Controllerboard", &controllerBoard);




  // Handle REST calls
  WiFiClient client = server.available();
  if (client)
  {

    while (!client.available()) {
      delay(1);
    }
    if (client.available())
    {




      rest.handle(client);

    }
  }
  // health indications for device
  rest.variable("ESP8266HeapSize", &heapSize);






  // Function to be exposed

  rest.function("arduino", jsonCmd); // For RasPiConnect - ArduinoConnect

  // auxillary functions
  rest.function("led", ledControl);
  rest.function("setID", setWeatherPlusIDControl);
  rest.function("resetOurWeather",   resetOurWeather);

  rest.function("setWUSID",   setWUSID);
  rest.function("setWUKEY",   setWUKEY);
  rest.function("setBAKEY",   setBAKEY);

  rest.function("setAdminPassword",   setAdminPassword);
  //rest.function("rebootOurWeather",   rebootOurWeather);
  rest.function("setDateTime",   setDateTime);
  rest.function("resetToDefaults",   resetToDefaults);

  rest.function("resetWiFiAccessPoint", resetWiFiAccessPoint);
  rest.function("updateOurWeather", updateOurWeather);



  // external interfaces

  rest.function("enableCWOP", enableCWOPControl);
  rest.function("enableTwitter", enableTwitterControl);

  // weather functions

  rest.function("WeatherSmall",   weatherSmallControl);
  rest.function("WeatherMedium",   weatherMediumControl);
  rest.function("WeatherLarge",   weatherLargeControl);
  rest.function("WeatherDemo",   weatherDemoControl);

  rest.function("EnglishUnits",   englishUnitControl);

  rest.function("MetricUnits",   metricUnitControl);

  // PubNub

  rest.function("EnablePubNub", enableDisableSDL2PubNub);

  rest.function("SendPubNubState", sendStateSDL2PubNub);


  // Thunderboard functions AS3935



  rest.function("setThunderBoardParams", setThunderBoardParams);





  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("OurWeather");



  initialize60MinuteRain();





  Serial.println();
  Serial.println();
  Serial.println("--------------------");
  Serial.println("OurWeather V2 ESP32");
  Serial.println("--------------------");
  Serial.print("Version: ");
  Serial.println(WEATHERPLUSESP32VERSION);

  Serial.print("Compiled at:");
  Serial.print (__TIME__);
  Serial.print(" ");
  Serial.println(__DATE__);

  //Serial.print("Free Sketch Space on OurWeather:");
  //Serial.println(ESP.getFreeSketchSpace());

  // test for SunAirPlus_Present
  SunAirPlus_Present = false;

  int MID = SunAirPlus.getManufID();


  if (MID != 0x5449)
  {
    SunAirPlus_Present = false;
    Serial.println("SunAirPlus Not Present");
  }
  else
  {
    SunAirPlus_Present = true;
    Serial.println("SunAirPlus Present");
  }

  // test for WXLink Present

  WXLink_Present = false;

  WXLink_Present = scanSerialForWXLink();

  WXLastMessageGood = false;

  WXMessageID = 0;
  WXLoadCurrent = 0.0;


  WXBatteryVoltage = 0.0;
  WXBatteryCurrent = 0.0;

  WXSolarPanelVoltage = 0.0;
  WXSolarPanelCurrent = 0.0;
  lastMessageID = -1;

  if (WXLink_Present == false)
  {

    Serial.println("WXLink Not Present");
  }
  else
  {

    Serial.println("WXLink Present");
    xSemaphoreGive( xSemaphoreReadWXLink);  // start the thread
  }


  Serial.print("port number = ");
  Serial.println(WEB_SERVER_PORT);

  delay(2000);

  ads1015.begin();

  adsAirQuality.begin();


  int16_t ad0 = adsAirQuality.readADC_SingleEnded(3);



  currentAirQuality = -1;
  currentAirQualitySensor = 0;
  INTcurrentAirQualitySensor = 0;

  if (ad0 != -1)
  {
    AirQualityPresent = true;
    Serial.println("AirQuality Extension Present");
  }
  else
  {
    AirQualityPresent = false;
    Serial.println("AirQuality Extension Not Present");
  }


  randomSeed(analogRead(0));


  lastBoot = now();



  Serial.print("OurWeather IP Address:");
  Serial.println(WiFi.localIP());
  myConnectedIp = WiFi.localIP();

  Serial.print("OurWeather Gateway Address:");
  myConnectedGateWay = WiFi.gatewayIP();
  Serial.println(WiFi.gatewayIP());

  Serial.print("OurWeather subnet Mask:");
  myConnectedMask = WiFi.subnetMask();
  Serial.println(WiFi.subnetMask());


  //blinkIPAddress();

  updateDisplay(DISPLAY_IPDISPLAY);

  // Now put PUBNUB Code up there

  updateDisplay(DISPLAY_SDL2PUBNUBCODE);


  timeElapsed = 0;


  // BMP280

  /* Initialise the sensor */
  if (!bme.begin())
  {

    Serial.println("BMP280 Not Found ");
    BMP280Found = false;

  }
  else
  {
    Serial.println("BMP280 Found");
    BMP280Found = true;
  }

  // Check for TSL2591

  TSL2591.parameter.I2CAddress = 0x29;

  //Here we can configure the TSL2591 Light Sensor
  //First we set the gain value
  //Higher gain values are better for dimmer light conditions, but lead to sensor saturation with bright light
  //We can choose among four gain values:

  //0b00:    Low gain mode
  //0b01:    Medium gain mode
  //0b10:    High gain mode
  //0b11:    Maximum gain mode

  TSL2591.parameter.gain = 0b01;

  //Longer integration times also helps in very low light situations, but the measurements are slower

  //0b000:   100ms (max count = 37888)
  //0b001:   200ms (max count = 65535)
  //0b010:   300ms (max count = 65535)
  //0b011:   400ms (max count = 65535)
  //0b100:   500ms (max count = 65535)
  //0b101:   600ms (max count = 65535)

  TSL2591.parameter.integration = 0b000;

  //The values for the gain and integration times are written transfered to the sensor through the function config_TSL2591
  //This function powers the device ON, then configures the sensor and finally powers the device OFF again

  //*********************************************************************
  //*************ADVANCED SETUP - SAFE TO IGNORE!************************

  //Now choose on which mode your device will run
  //On doubt, just leave on normal mode, that's the default value

  //0b00:     In sleep mode no measurements are performed, but power consumption is at a minimum
  //0b01:     In forced mode a single measured is performed and the device returns automatically to sleep mode
  //0b11:     In normal mode the sensor measures continually (default value)



  TSL2591.config_TSL2591();

  if (TSL2591.init_TSL2591() == 0x50)
  {
    Serial.println("TSL2591 Found");
    TSL2591_Present = true;

    Serial.print(F("Illuminance in Lux:\t\t"));
    TSL2591_Lux = TSL2591.readIlluminance_TSL2591();
    Serial.println(TSL2591_Lux);



  }
  else
  {
    Serial.println("TSL2591 Not Found");
    TSL2591_Present = false;
  }



  SHT30_Temperature = 0.0;

  SHT30_Humidity = 0.0;
  dewpoint = 0.0;

  // Check for SHT30
  int sht30_success;

  sht30_success = sht30.get();

  if (sht30_success == 0)
  {
    SHT30_Present = true;
    Serial.println("SHT30 Found");
    Serial.print("SHT30Temp=");
    Serial.println(sht30.cTemp);
    Serial.print("SHT30Humid=");
    Serial.println(sht30.humidity);



  }
  else
  {
    Serial.println("SHT30 Not Found");
    SHT30_Present = false;
  }

  if (WiFiPresent == true)
  {

    PubNub.begin(SDL2PubNubCode.c_str(), SDL2PubNubCode_Sub.c_str());

    Serial.println("PubNub set up");
  }



  if (UseBlynk == true)
  {



    Blynk.config(BlynkAuthCode.c_str());

    // Setup a function to be called every 10 seconds
    Btimer.setInterval(5000L, myBTimerEvent);




    Blynk.connect();




    if (Blynk.connected())
    {
      Serial.println("Blynk Connected");

    }
    else
    {
      Serial.println("Blynk NOT Connected");

    }

    clearSolar();  // Clear the solar data
    // Clear the terminal content
    //statusTerminal.clear();
    writeToStatusLine((String)"OurWeather Version V" + (String)WEATHERPLUSESP32VERSION + " Started");

    writeToBlynkStatusTerminal((String)"OurWeather Version V" + (String)WEATHERPLUSESP32VERSION + " Started");
    // Print out the presents
    if (SunAirPlus_Present)
    {
      writeToBlynkStatusTerminal("SunAirPlus Present");

    }
    else
    {
      writeToBlynkStatusTerminal("SunAirPlus Not Present");
    }

    if (WXLink_Present)
    {
      writeToBlynkStatusTerminal("WXLink Present");

    }
    else
    {
      writeToBlynkStatusTerminal("WXLink Not Present");
    }

    if (AirQualityPresent)
    {
      writeToBlynkStatusTerminal("Air Quality Sensor Present");

    }
    else
    {
      writeToBlynkStatusTerminal("Air Quality Sensor Not Present");
    }

    if (BMP280Found)
    {
      writeToBlynkStatusTerminal("BMP280 Present");

    }
    else
    {
      writeToBlynkStatusTerminal("BMP280 Not Present");
    }


    if (TSL2591_Present)
    {
      writeToBlynkStatusTerminal("TSL2591 Present");

    }
    else
    {
      writeToBlynkStatusTerminal("TSL2591 Not Present");
    }

    if (SHT30_Present)
    {
      writeToBlynkStatusTerminal("SHT30 Present");

    }
    else
    {
      writeToBlynkStatusTerminal("SHT30 Not Present");
    }


    if (AS3935Present)
    {
      writeToBlynkStatusTerminal("AS3935 ThunderBoard Present");

    }
    else
    {
      writeToBlynkStatusTerminal("AS3935 ThunderBoard Not Present");
    }


    if (EnglishOrMetric == 0)
    {
      Blynk.virtualWrite(V8,  "English");
      writeToBlynkStatusTerminal("Units set to English");
    }
    else
    {
      Blynk.virtualWrite(V8,  "Metric");
      writeToBlynkStatusTerminal("Units set to Metric ");




    }
  }
  else
  {

    // Do No Blynk Setup

  }

  // Turn on display loop
  xSemaphoreGive( xSemaphoreReadSensor);   // turn it on



} // end setup


//
//
// loop()
//
//


void loop() {

  /*
    // put your main code here, to run repeatedly:
    //Serial.println("Starting Main Loop");
    // Handle REST calls
    WiFiClient client = server.available();

    int timeout;
    timeout = 0;
    if (client)
    {

    // Thank you to MAKA69 for this suggestion
    while (!client.available()) {
      Serial.print(".");
      delay(1);
      timeout++;
      if (timeout > 1000) {
        Serial.print("INFINITE LOOP BREAK!");
        break;
      }
    }
    if (client.available())
    {




      rest.handle(client);

    }
    }
    client.stop();



  */

  if (UseBlynk)
  {

    Blynk.run();
    Btimer.run(); // Initiates BlynkTimer
  }
  else
  {

    // no blynk
  }
  yield();

}
