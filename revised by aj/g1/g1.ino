#include <SoftwareSerial.h>
//The connection for  Attiny & BMP180 are  SDA pin 5 ,SCL pin 7 for I2C 
#include  <TinyWireM.h>
#define BMP180_ADDRESS 0x77  // I2C address of BMP180   
// define calibration data for temperature:
int ac1;
int ac2; 
int ac3; 
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1; 
int b2;
int mb;
int mc;
int md;
long b5; 
//define variables for pressure and temperature calculation
long x1,x2;
//define variables for pressure calculation
long x3,b3,b6,p;
unsigned long b4,b7;
//define variables for temperature and pressure reading

short temperature;
long pressure;
const unsigned char OSS = 3;  // Oversampling Setting
/* blz 12 Datasheet
OSS=0 ultra Low Power Setting, 1 sample, 4.5 ms 3uA
OSS=1 Standard Power Setting, 2 samples, 7.5 ms 5uA
OSS=2 High Resolution,              4 samples, 13.5 ms 7uA
OSS=3 Ultra High Resolution,    2 samples, 25.5 ms 12uA
*/
#define rxPin 3
#define txPin 4

SoftwareSerial mySerial(rxPin, txPin);


void setup() {
  // put your setup code here, to run once:
  // First read calibration data from EEPROM
  ac1 = bmp180ReadInt(0xAA);
  ac2 = bmp180ReadInt(0xAC);
  ac3 = bmp180ReadInt(0xAE);
  ac4 = bmp180ReadInt(0xB0);
  ac5 = bmp180ReadInt(0xB2);
  ac6 = bmp180ReadInt(0xB4);
  b1 = bmp180ReadInt(0xB6);
  b2 = bmp180ReadInt(0xB8);
  mb = bmp180ReadInt(0xBA);
  mc = bmp180ReadInt(0xBC);
  md = bmp180ReadInt(0xBE);
  
 mySerial.begin(115200);

}

void loop() {


 temperature = bmp180CorrectTemperature(bmp180ReadUT());
 pressure = bmp180CorrectPressure(bmp180ReadUP());
  // put your main code here, to run repeatedly:
  mySerial.print("dafu:");
  mySerial.print(pressure);
  mySerial.println();
  delay(500);
   
  
}
double bmp180CorrectTemperature(unsigned int ut)
{
  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);  
  b5 = x1 + x2; 
  return (((b5 + 8)>>4));  
}
int bmp180ReadInt(unsigned char address)
{
  unsigned char msb, lsb;
  TinyWireM.beginTransmission(BMP180_ADDRESS);
  TinyWireM.send(address);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(BMP180_ADDRESS, 2);
  while(TinyWireM.available()<2);
  msb = TinyWireM.receive();
  lsb = TinyWireM.receive();
  return (int) msb<<8 | lsb;
}


unsigned int bmp180ReadUT()
{
  unsigned int ut;
  
  // Write 0x2E into Register 0xF4 and wait at least 4.5mS
  // This requests a temperature reading 
  // with results in 0xF6 and 0xF7
  TinyWireM.beginTransmission(BMP180_ADDRESS);
  TinyWireM.send(0xF4);
  TinyWireM.send(0x2E);
  TinyWireM.endTransmission();
  
  // Wait at least 4.5ms
  delay(5);
  
  // Then read two bytes from registers 0xF6 (MSB) and 0xF7 (LSB)
  // and combine as unsigned integer
  ut = bmp180ReadInt(0xF6);
  return ut;
}

// Read the uncompensated pressure value
unsigned long bmp180ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;
  
  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  TinyWireM.beginTransmission(BMP180_ADDRESS);
  TinyWireM.send(0xF4);
  TinyWireM.send(0x34 + (OSS<<6));
  TinyWireM.endTransmission();
  
  // Wait for conversion, delay time dependent on OSS
  delay(5 + (8*OSS));
  
  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  TinyWireM.beginTransmission(BMP180_ADDRESS);
  TinyWireM.send(0xF6);
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(BMP180_ADDRESS, 3);
  
  // Wait for data to become available
  while(TinyWireM.available() < 3)
    ;
  msb = TinyWireM.receive();
  lsb = TinyWireM.receive();
  xlsb = TinyWireM.receive();
  
  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);
  
  return up;
}

double bmp180CorrectPressure(unsigned long up)
{ 
  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;
  
  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;
  
  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
  p = (b7/b4)<<1;   
  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;
  
  return p;
}

