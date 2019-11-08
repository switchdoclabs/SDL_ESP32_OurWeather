//
// Non volalile preferences functions
//


// Global preference variables

unsigned int currentLEDMode;
unsigned int OLEDOn;


unsigned int darkLight;

String WiFi_SSID = "";
String WiFi_psk = "";

int ClockTimeOffsetToUTC = 0;



void resetPreferences()
{
  preferences.begin("OWinit", false);
  preferences.clear();
  preferences.end();

  ClockTimeOffsetToUTC = DEFAULTCLOCKTIMEOFFSETTOUTC;
  Serial.println("----Clearing Preferences---");


}

void writePreferences()
{
  preferences.begin("OWinit", false);




  preferences.putString("WiFi_SSID", WiFi_SSID);
  preferences.putString("WiFi_psk", WiFi_psk);
  preferences.putInt("OLEDOn", OLEDOn);

  preferences.putString("stationName", stationName);

  Serial.print("ClockTimeOffsetToUTC=");
  Serial.println(ClockTimeOffsetToUTC);
  preferences.putInt("COffsetToUTC", ClockTimeOffsetToUTC);

  preferences.putInt("EnglishOrMetric", EnglishOrMetric);
  preferences.putInt("WeatherDisplayMode", WeatherDisplayMode);

  preferences.putFloat("altitude_meters", altitude_meters);
  preferences.putString("adminPassword", adminPassword);

  preferences.putString("WeatherUnderground_StationID", WeatherUnderground_StationID);
  preferences.putString("WeatherUnderground_StationKey", WeatherUnderground_StationKey);

  preferences.putInt("pubNubEnabled", pubNubEnabled);
  preferences.putString("SDL2PubNubCode", SDL2PubNubCode);
  preferences.putString("SDL2PubNubCode_Sub", SDL2PubNubCode_Sub);

  preferences.putString("BlynkAuthCode", BlynkAuthCode);
  preferences.putString("as3935_Params", as3935_Params);


  preferences.end();

#ifdef OWDEBUG
  Serial.println("----Writing Preferences---");


  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);

  Serial.print("OLEDOn="); Serial.println(OLEDOn);

  Serial.printf("stationName="); Serial.println(stationName);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);

  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("WeatherDisplayMode:");
  Serial.println(WeatherDisplayMode);
  Serial.print("Station Name:");
  Serial.println(stationName);
  Serial.print("Altitude:");
  Serial.println(altitude_meters);
  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");
  Serial.print("WeatherUnderground Station ID:");
  Serial.println(WeatherUnderground_StationID);
  Serial.print("WeatherUnderground Station KEY:");
  Serial.println(WeatherUnderground_StationKey.substring(0, 2) + "******");
  Serial.print("BlynkAuthCode:");
  Serial.println(BlynkAuthCode);

  Serial.print("pubNubEnabled=");
  Serial.println(pubNubEnabled);
  Serial.print("SDL2PubNubCode=");
  Serial.println(SDL2PubNubCode);
  Serial.print("SDL2PubNubCode_Sub=");
  Serial.println(SDL2PubNubCode_Sub);
  Serial.print("as3935_Params=");
  Serial.println(as3935_Params);
  Serial.println("--------------------------");

#endif


}

void readPreferences()
{
  preferences.begin("OWinit", false);
  OLEDOn = preferences.getInt("OLEDOn", 1);

  WiFi_SSID = preferences.getString("WiFi_SSID", "");
  WiFi_psk = preferences.getString("WiFi_psk", "");

  stationName =   preferences.getString("stationName", "");

  ClockTimeOffsetToUTC = preferences.getInt("COffsetToUTC", DEFAULTCLOCKTIMEOFFSETTOUTC);


  EnglishOrMetric = preferences.getInt("EnglishOrMetric");
  WeatherDisplayMode = preferences.getInt("WeatherDisplayMode", DISPLAY_WEATHER_LARGE);

  altitude_meters = preferences.getFloat("altitude_meters", 637.0);
  adminPassword = preferences.getString("adminPassword", "admin");

  WeatherUnderground_StationID = preferences.getString("WeatherUnderground_StationID", "");
  WeatherUnderground_StationKey = preferences.putString("WeatherUnderground_StationKey", "");

  pubNubEnabled = preferences.getInt("pubNubEnabled");
  SDL2PubNubCode = preferences.getString("SDL2PubNubCode", "XX");
  SDL2PubNubCode_Sub = preferences.getString("SDL2PubNubCode_Sub", "XX");

  BlynkAuthCode =   preferences.getString("BlynkAuthCode", "");
  
#ifdef OWDEBUG
  Serial.print("BlynkAuthCode=");
  Serial.println(BlynkAuthCode);
  Serial.print("BAC Length=");
  Serial.println(BlynkAuthCode.length());
#endif
  if (BlynkAuthCode.length() <4)   // Short blynk codes are not valid
    UseBlynk = false;
  else
    UseBlynk = true;



  as3935_Params = preferences.getString("as3935_Params", "");

  preferences.end();

#ifdef OWDEBUG
  Serial.println("----Reading Preferences---");

  Serial.printf("SSID="); Serial.println(WiFi_SSID);
  Serial.printf("psk="); Serial.println(WiFi_psk);
  Serial.print("OLEDOn="); Serial.println(OLEDOn);
  Serial.printf("stationName="); Serial.println(stationName);
  Serial.printf("COffsetToUTC="); Serial.println(ClockTimeOffsetToUTC);

  Serial.print("EnglishOrMetric:");
  Serial.println(EnglishOrMetric);
  Serial.print("WeatherDisplayMode:");
  Serial.println(WeatherDisplayMode);
  Serial.print("Station Name:");
  Serial.println(stationName);
  Serial.print("Altitude:");
  Serial.println(altitude_meters);
  Serial.print("Admin Password:");
  Serial.println(adminPassword.substring(0, 2) + "******");
  Serial.print("WeatherUnderground Station ID:");
  Serial.println(WeatherUnderground_StationID);
  Serial.print("WeatherUnderground Station KEY:");
  Serial.println(WeatherUnderground_StationKey.substring(0, 2) + "******");
  Serial.print("BlynkAuthCode:");
  Serial.println(BlynkAuthCode);

  Serial.print("pubNubEnabled=");
  Serial.println(pubNubEnabled);
  Serial.print("SDL2PubNubCode=");
  Serial.println(SDL2PubNubCode);
  Serial.print("SDL2PubNubCode_Sub=");
  Serial.println(SDL2PubNubCode_Sub);
  Serial.print("as3935_Params=");
  Serial.println(as3935_Params);
  Serial.println("--------------------------");
#endif
}
