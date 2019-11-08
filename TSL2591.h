

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#define TSL2591_CHIP_ID			0x12
#define TSL2591_COMMAND_BIT		0x80
#define TSL2591_NORMAL_MODE		0x20
#define TSL2591_ENABLE_ADDR		0x00
#define TSL2591_CONFIG_ADDR		0x01
#define TSL2591_C0DATAL_ADDR	0x14
#define TSL2591_C0DATAH_ADDR	0x15
#define TSL2591_C1DATAL_ADDR	0x16
#define TSL2591_C1DATAH_ADDR	0x17


 

struct DeviceParameter
{
  uint8_t communication = 0;
  uint8_t I2CAddress;
  int8_t integration;
  int16_t gain;
  
};

class TSL2591 
{
 public: 
  
  DeviceParameter parameter;

  int32_t t_fine;
  
  TSL2591();
  uint8_t readByte(byte reg);
  uint16_t readByte16(byte reg);
  void writeByte(byte reg, byte value);
  
  uint8_t init_TSL2591(void);
  uint8_t checkID_TSL2591(void);
  void enable_TSL2591(void);
  void disable_TSL2591(void);
  void config_TSL2591(void);
  uint32_t getFullLuminosity_TSL2591(void);
  uint16_t getFullSpectrum_TSL2591(void);
  uint16_t getInfrared_TSL2591(void);
  uint16_t getVisibleLight_TSL2591(void);
  float readIlluminance_TSL2591(void);
  
  float calculateLux_TSL2591 (uint16_t ch0, uint16_t ch1);
  uint32_t getLux_TSL2591(void);
  
 

};
