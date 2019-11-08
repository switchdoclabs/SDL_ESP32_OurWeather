




#if defined(_AVR_)
#include <util/delay.h>
#endif

#include "TSL2591.h"
#include "Wire.h"

TSL2591::TSL2591()
{
  parameter.communication;
  parameter.I2CAddress;
  parameter.integration;
  parameter.gain;

}




uint8_t TSL2591::init_TSL2591(void)
{
	return checkID_TSL2591();
}

//##########################################################################
//SET UP FUNCTIONS - TSL2591
//##########################################################################
uint8_t TSL2591::checkID_TSL2591(void)
{
	//In order to read the register TSL2591_CHIP_ID we need the readByte function
	//The register address "reg" is an 8-bit address and is composed of three variables
	//CMD (bit 7), TRANSACTION (bits 6 and 5) and ADDRESS (bits 4, 3, 2, 1 and 0)
	//The CMD or command bit must be set to 1 (0b1000000 or 0x80)
	//TRANSACTION is set here to Normal Operation by writing the bits 6 and 5 to 0 and 1 respectively (0b00100000 or 0x20)
	//ADDRESS represents the chip ID address and is defined as 0x12 or 0b00010010	
	//So by using the OR function to add all three values together we get the register "reg"

	//readByte(reg)
	//reg = (CMD | TRANSACTION | ADDRESS)
	//reg = TSL2591_COMMAND_BIT (0x80) | TSL2591_NORMAL_MODE (0x20) | TSL2591_CHIP_ID (0x12)
	
	uint8_t chipID;
	chipID = readByte(TSL2591_COMMAND_BIT | TSL2591_NORMAL_MODE | TSL2591_CHIP_ID);
	return chipID;
	
}
//##########################################################################
void TSL2591::enable_TSL2591(void)
{
	//The enable_TSL2591 function is used to power the device ON by writting the ENABLE register	
	//The CMD and TRANSACTION values are the same as for the reading the Chip ID 
	//Therefore the Command Bit (CMD) is set to 1 and TRANSACTION is set to Normal Mode (0x20)
	//ADDRESS represents the address of the ENABLE register (0x00)
	
	//writeByte(reg, val)
	//reg = (CMD | TRANSACTION | ADDRESS)
	//reg = TSL2591_COMMAND_BIT (0x80) | TSL2591_NORMAL_MODE (0x20) | TSL2591_ENABLE_ADDR (0x00)
	//val =  enable_AIEN (0x10) | enable_AEN (0x02) | enable_powerON (0x01)
	
	writeByte(TSL2591_COMMAND_BIT | TSL2591_NORMAL_MODE | TSL2591_ENABLE_ADDR, 0x10 | 0x02 | 0x01);
}
//##########################################################################
void TSL2591::disable_TSL2591(void)
{
	//The disable_TSL2591 is used to power the device OFF	

	//writeByte(reg, val)
	//reg = (CMD | TRANSACTION | ADDRESS)
	//reg = TSL2591_COMMAND_BIT (0x80) | TSL2591_NORMAL_MODE (0x20) | TSL2591_ENABLE_ADDR (0x00)
	//val = enable_powerOFF (0x00)
	
	writeByte(TSL2591_COMMAND_BIT | TSL2591_NORMAL_MODE | TSL2591_ENABLE_ADDR, 0x00);
}
//##########################################################################
void TSL2591::config_TSL2591(void)
{
	//Here we set the gain and the integration time of the photodiode channels
	//There are four possible gain modes: low, medium, high and maximum
	//The integration time can be set to 100, 200, 300, 400, 500 and 600 ms
	//Longer integration times lead to more light sensitivity, but also to longer measurements	
	
	//First we power the device ON
	enable_TSL2591();	
	
	//Now we set the values for the GAIN and the INTEGRATION TIME
	//writeByte(reg, val)
	//reg = (CMD | TRANSACTION | ADDRESS)
	//reg = TSL2591_COMMAND_BIT (0x80) | TSL2591_NORMAL_MODE (0x20) | TSL2591_CONFIG_ADDR (0x01)
	//val = GAIN | INTEGRATION	
	byte value;	
	value = (parameter.gain << 4) & 0b00110000;
	value |= parameter.integration & 0b00000111;	
	writeByte(TSL2591_COMMAND_BIT | TSL2591_NORMAL_MODE | TSL2591_CONFIG_ADDR, value);
	
	//Finally we power the device OFF
	disable_TSL2591();	
	
}
//##########################################################################
//DATA READOUT FUNCTIONS - TSL2591
//##########################################################################
uint32_t TSL2591::getFullLuminosity_TSL2591(void)
{
  
  //The first step is powering the device ON 
  enable_TSL2591();

  //Wait x ms for ADC to complete
  for (uint8_t x=0; x<=parameter.integration; x++) 
  {
    delay(120);
  }
    
  //Now we read the ALS Data Register (0x14 - 0x17)
  //Data is stored as two 16-bit values, one for each Photodiode Channel
  //Here we read both values and write them as a single 32-bit value
  uint32_t y;
  y = readByte16(TSL2591_COMMAND_BIT | TSL2591_NORMAL_MODE | TSL2591_C1DATAL_ADDR);
  y <<= 16;
  y |= readByte16(TSL2591_COMMAND_BIT | TSL2591_NORMAL_MODE | TSL2591_C0DATAL_ADDR);


  //Finally we power the device OFF
  disable_TSL2591();

  return y;
}
//##########################################################################
uint16_t TSL2591::getFullSpectrum_TSL2591(void)
{
  uint32_t x = getFullLuminosity_TSL2591();
  
  return (x & 0xFFFF);
}
//##########################################################################
uint16_t TSL2591::getInfrared_TSL2591(void)
{
  uint32_t x = getFullLuminosity_TSL2591();
  
  return (x >> 16);
}
//##########################################################################
uint16_t TSL2591::getVisibleLight_TSL2591(void)
{
  uint32_t x = getFullLuminosity_TSL2591();
  
  return ((x & 0xFFFF) - (x >> 16));
}
//##########################################################################
float TSL2591::calculateLux_TSL2591(uint16_t ch0, uint16_t ch1)
{
  uint16_t atime, again;
  float    cpl, lux1, lux2, lux;

  //default
  atime = 100.0F;
  again = 25.0F;
  
  switch (parameter.integration)
  {
	case 0b000 :
	atime = 100.0F;
	break;
	
	case 0b001 :
	atime = 200.0F;
	break;
    
	case 0b010 :
	atime = 300.0F;
	break;
	
	case 0b011 :
	atime = 400.0F;
	break;
	
	case 0b100 :
	atime = 500.0F;
	break;
	
	case 0b101 :
	atime = 600.0F;
	break;    
  } 
  
  switch (parameter.gain)
  {
	case 0b00 :
	again = 1.0F;
	break;
	
	case 0b01 :
	again = 25.0F;
	break;
	
	case 0b10 :
	again = 428.0F;
	break;
	
	case 0b11 :
	again = 9876.0F;
	break;
  
  }
  
  
  cpl = (atime * again)/408.0F;
  lux1 = ( (float)ch0 - (2 * (float)ch1) ) / cpl;
  
  return (float)ch0;
}

//##########################################################################
float TSL2591::readIlluminance_TSL2591(void)
{
  uint32_t lum = getFullLuminosity_TSL2591();
  uint16_t ir, full;  
  float lux;
  
  ir = lum >> 16;
  full = lum & 0xFFFF;
  
  lux = calculateLux_TSL2591(full, ir);
  return lux;

}


//##########################################################################
//BASIC FUNCTIONS - TSL2591
//##########################################################################
void TSL2591::writeByte(byte reg, byte value)
{	
	
	Wire.beginTransmission(parameter.I2CAddress);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission();
	
}
//##########################################################################
uint8_t TSL2591::readByte(byte reg)
{
	uint8_t value;
	
	Wire.beginTransmission(parameter.I2CAddress);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(parameter.I2CAddress,1);		
	value = Wire.read();		
	return value;
	
}
//##########################################################################
uint16_t TSL2591::readByte16(byte reg)
{
	uint16_t value1;
	uint16_t value2;
	
	Wire.beginTransmission(parameter.I2CAddress);
	Wire.write(reg);
	Wire.endTransmission();
	
	Wire.requestFrom(parameter.I2CAddress,2);		
	value2 = Wire.read();	
	value1 = Wire.read();
	
	value1 <<= 8;
	value1 |= value2;
	
	return value1;
	
}
