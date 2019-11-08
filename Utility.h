// Utility functions


float returnPercentLeftInBattery(float currentVoltage, float maxVolt)
{

  float returnPercent;

  float scaledVolts = currentVoltage / maxVolt;

  if (scaledVolts > 1.0)
    scaledVolts = 1.0;


  if (scaledVolts > .9686)
  {
    returnPercent = 10 * (1 - (1.0 - scaledVolts) / (1.0 - .9686)) + 90;
    return returnPercent;
  }

  if (scaledVolts > 0.9374)
  {
    returnPercent = 10 * (1 - (0.9686 - scaledVolts) / (0.9686 - 0.9374)) + 80;
    return returnPercent;
  }


  if (scaledVolts > 0.9063)
  {
    returnPercent = 30 * (1 - (0.9374 - scaledVolts) / (0.9374 - 0.9063)) + 50;
    return returnPercent;

  }

  if (scaledVolts > 0.8749)
  {
    returnPercent = 20 * (1 - (0.8749 - scaledVolts) / (0.9063 - 0.8749)) + 11;

    return returnPercent;
  }


  if (scaledVolts > 0.8437)
  {
    returnPercent = 15 * (1 - (0.8437 - scaledVolts) / (0.8749 - 0.8437)) + 1;
    return returnPercent;

  }

  if (scaledVolts > 0.8126)
  {
    returnPercent = 7 * (1 - (0.8126 - scaledVolts) / (0.8437 - 0.8126)) + 2;
    return returnPercent;
  }


  if (scaledVolts > 0.7812)
  {
    returnPercent = 4 * (1 - (0.7812 - scaledVolts) / (0.8126 - 0.7812)) + 1;
    return returnPercent;
  }

  return 0;
}


String returnDateTime(time_t myTime)
{

char buff[40];

struct tm *myTm;
myTm = localtime(&myTime);
strftime(buff, 20, "%Y-%m-%d %H:%M:%S", myTm);
String returnString;
returnString = String(buff);

return returnString;

}


void blinkLED(int timesToBlink, int delayBetweenBlinks)
{

  int i;
  for (i = 0; i < timesToBlink; i++)
  {
    digitalWrite(BLINKPIN, 1);
    delay(delayBetweenBlinks/2);

    digitalWrite(BLINKPIN, 0);
    delay(delayBetweenBlinks/2);
  }

}
#include <rom/rtc.h>

void print_reset_reason(RESET_REASON reason)
{
  switch ( reason)
  {
    case 1 : Serial.println ("POWERON_RESET"); break;         /**<1, Vbat power on reset*/
    case 3 : Serial.println ("SW_RESET"); break;              /**<3, Software reset digital core*/
    case 4 : Serial.println ("OWDT_RESET"); break;            /**<4, Legacy watch dog reset digital core*/
    case 5 : Serial.println ("DEEPSLEEP_RESET"); break;       /**<5, Deep Sleep reset digital core*/
    case 6 : Serial.println ("SDIO_RESET"); break;            /**<6, Reset by SLC module, reset digital core*/
    case 7 : Serial.println ("TG0WDT_SYS_RESET"); break;      /**<7, Timer Group0 Watch dog reset digital core*/
    case 8 : Serial.println ("TG1WDT_SYS_RESET"); break;      /**<8, Timer Group1 Watch dog reset digital core*/
    case 9 : Serial.println ("RTCWDT_SYS_RESET"); break;      /**<9, RTC Watch dog Reset digital core*/
    case 10 : Serial.println ("INTRUSION_RESET"); break;      /**<10, Instrusion tested to reset CPU*/
    case 11 : Serial.println ("TGWDT_CPU_RESET"); break;      /**<11, Time Group reset CPU*/
    case 12 : Serial.println ("SW_CPU_RESET"); break;         /**<12, Software reset CPU*/
    case 13 : Serial.println ("RTCWDT_CPU_RESET"); break;     /**<13, RTC Watch dog Reset CPU*/
    case 14 : Serial.println ("EXT_CPU_RESET"); break;        /**<14, for APP CPU, reseted by PRO CPU*/
    case 15 : Serial.println ("RTCWDT_BROWN_OUT_RESET"); break; /**<15, Reset when the vdd voltage is not stable*/
    case 16 : Serial.println ("RTCWDT_RTC_RESET"); break;     /**<16, RTC Watch dog reset digital core and rtc module*/
    default : Serial.println ("NO_MEAN");
  }
}


void SetRandomSeed()
{
  uint32_t seed;

  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  vTaskDelay(1 / portTICK_PERIOD_MS);

  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }

  // Serial.println(seed);
  randomSeed(seed);
}
