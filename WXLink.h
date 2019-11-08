//
// Supports the SwitchDoc Labs WXLink Device
//
#include <RH_RF95.h>

#define RXDEBUG


byte buffer[75];
byte lastGoodMessage[64];
byte buflen = 0;

long consecutiveGoodMessages;
long lastGoodMessageID;
long goodMessages;
long badMessages;

#define COMSerial Serial2


RH_RF95<HardwareSerial> rf95(COMSerial);

bool scanSerialForWXLink()
{

  
  if (!rf95.init())
  {
    Serial.println("WXLink init failed");
    return false;

  }

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  //rf95.setTxPower(13, false);

  rf95.setFrequency(434.0);
  rf95.setTxPower(13);
  int Bw31_25Cr48Sf512 = 2;

  rf95.setModemConfig(RH_RF95<HardwareSerial>::ModemConfigChoice(Bw31_25Cr48Sf512));
  //rf95.printRegisters();
  //rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);
  //rf95.setModemConfig(RH_RF95::Bw125Cr48Sf4096);

  //rf95.setTxPower(5);

  //rf95.printRegisters();

  consecutiveGoodMessages = 0;
  int i;
  for (i = 0; i < 64; i++)
  {
    lastGoodMessage[i] = 0;


  }
  return true;

}







void printBuffer(byte *buffer, int buflen)
{
  int i;
  for (i = 0; i < buflen; i++)
  {
#ifdef RXDEBUG
    Serial.print("i=");
    Serial.print(i);
    Serial.print(" | ");
    Serial.println(buffer[i], HEX);
#endif

  }

}


int convert2BytesToInt(byte *buffer, int bufferStart)
{

  union u_tag {
    byte b[2];
    int fval;
  } u;

  u.b[0] = buffer[bufferStart];
  u.b[1] = buffer[bufferStart + 1];


  return u.fval;

}

long convert4BytesToLong(byte *buffer, int bufferStart)
{

  union u_tag {
    byte b[4];
    long fval;
  } u;

  u.b[0] = buffer[bufferStart];
  u.b[1] = buffer[bufferStart + 1];
  u.b[2] = buffer[bufferStart + 2];
  u.b[3] = buffer[bufferStart + 3];

  return u.fval;

}

float convert4BytesToFloat(byte *buffer, int bufferStart)
{
  union u_tag {
    byte b[4];
    float fval;
  } u;

  u.b[0] = buffer[bufferStart];
  u.b[1] = buffer[bufferStart + 1];
  u.b[2] = buffer[bufferStart + 2];
  u.b[3] = buffer[bufferStart + 3];

  return u.fval;


}

int interpretBuffer(byte *buffer, int buflen)
{
  if (!((buffer[0] == 0xAB) && (buffer[1] == 0x66)))
  {
    // start bytes are not in buffer - reject
    return 1; // no start bytes
  }
#ifdef RXDEBUG
  Serial.println("Start Bytes Found");
  Serial.print("Bufflen = ");
  Serial.println(buflen);
#endif
  if (buflen != 63)
  {
    return 2; // buflen wrong
  }
  unsigned short checksumValue;

  // calculate checksum
  checksumValue = crc.XModemCrc(buffer, 0, 59);
#ifdef RXDEBUG
  Serial.print("crc = 0x");
  Serial.println(checksumValue, HEX);

  Serial.print("receivedChecksum=");
  Serial.print(buffer[61], HEX);
  Serial.println(buffer[62], HEX);
#endif

  if ((checksumValue >> 8) != buffer[61])
  {
    // bad checksum
    return 3;  // bad checksum

  }
  if ((checksumValue & 0xFF) != buffer[62])
  {
    // bad checksum
    return 3;  // bad checksum

  }

  int ProtocolID = buffer[2] / 10;

  if (ProtocolID != 3)
  {
    #ifdef RXDEBUG
      Serial.print("Received Protocol ");
      Serial.println(ProtocolID);
    #endif
    // unknown protocol ignore
    return 4;
  }

  //

#ifdef RXDEBUG
  //

  Serial.println();
  Serial.println(F("-------------"));
  Serial.println(F("-------------"));

  Serial.print(F("ProtocolByte="));
  Serial.println(buffer[2]);

  Serial.print(F("ProtocolID="));
  Serial.println(buffer[2] / 10);
  Serial.print(F("Protocol Software Version="));
  Serial.println(buffer[2] - (buffer[2] / 10) * (10));

  Serial.print(F("TimeSinceReboot(msec)="));
  Serial.println(convert4BytesToLong(buffer, 3));

  Serial.print(F("Wind Direction="));
  Serial.println(convert2BytesToInt(buffer, 7));

  Serial.print(F("Average Wind Speed (KPH)="));
  Serial.println(convert4BytesToFloat(buffer, 9));

  Serial.print(F("Wind Clicks="));
  Serial.println(convert4BytesToLong(buffer, 13));

  Serial.print(F("Total Rain Clicks="));
  Serial.println(convert4BytesToLong(buffer, 17));

  Serial.print(F("Max Wind Gust="));
  Serial.println(convert4BytesToFloat(buffer, 21));



  Serial.print(F("Outside Temperature="));
  Serial.println(convert4BytesToFloat(buffer, 25));

  Serial.print(F("OT Hex="));
  Serial.print(buffer[25], HEX);
  Serial.print(buffer[26], HEX);
  Serial.print(buffer[27], HEX);
  Serial.println(buffer[28], HEX);

  Serial.print(F("Outside Humidity="));
  Serial.println(convert4BytesToFloat(buffer, 29));

  Serial.print(F("BatteryVoltage="));
  Serial.println(convert4BytesToFloat(buffer, 33));
  Serial.print(F("BatteryCurrent="));
  Serial.println(convert4BytesToFloat(buffer, 37));
  Serial.print(F("LoadCurrent="));
  Serial.println(convert4BytesToFloat(buffer, 41));
  Serial.print(F("SolarPanelVoltage="));
  Serial.println(convert4BytesToFloat(buffer, 45));
  Serial.print(F("SolarPanelCurrent="));
  Serial.println(convert4BytesToFloat(buffer, 49));

  Serial.print(F("AuxA="));
  Serial.println(convert4BytesToFloat(buffer, 53));

  Serial.print(F("Message ID="));
  Serial.println(convert4BytesToLong(buffer, 57));


  Serial.print(F("Checksum High=0x"));
  Serial.println(buffer[61], HEX);
  Serial.print(F("Checksum Low=0x"));
  Serial.println(buffer[62], HEX);



#endif

  return 0;

}

void clearBufferArray(int buflen)              // function to clear buffer array
{
  for (int i = 0; i < buflen; i++)
  {
    buffer[i] = NULL; // clear all index of array with command NULL
  }
}


bool readWXLink()
{

  buflen = 75;
  byte messageLength;
  if (rf95.recv(buffer, &buflen))
  {
    Serial.println("Message Received");
  }
  else
  {
    Serial.println("No Message Received");
  }

  messageLength = buflen;   // clear off LoRa corruption byte

  //printBuffer( buffer, buflen);


  int interpretResult = interpretBuffer(buffer, buflen);


  switch (interpretResult)
  {
    case 0:
      {
        Serial.println("Good Message");

        return true;

      }
      break;
    case 1:
      Serial.println("Bad Message - No Start Bytes");

      return false;
      break;
    case 2:
      Serial.println("Bad Message - buffer length incorrect");
      return false;
      break;
    case 3:
      Serial.println("Bad Message - Bad Checksum");
      return false;
      break;
    case 4:
      Serial.println("Good Message - Unknown Protocol");
      return false;
      break;
    default:

      Serial.print("Bad Message - Unknown Return Code =");
      Serial.println(interpretResult);
      return false;
      break;
  }





  int i;

  // digitalWrite(LED, HIGH);
  //delay(100);
  //digitalWrite(LED, LOW);

  return false;


}
