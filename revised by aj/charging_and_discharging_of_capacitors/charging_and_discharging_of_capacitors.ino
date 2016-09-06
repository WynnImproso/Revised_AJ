//VISSER program for charging and discharging of capacitor
// data can be stored in sd card
#include <SPI.h>
#include <vLib.h>
vLib vlib;
#include <SD.h>
#include <stdio.h>
//#include<LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);

const int analogInPin = A1;  // Analog input pin that the potentiometer is attached to

int sensorValue = 0;    // value read from the pot
long int start;
float volts=0.0;
long ttime=0;

void setup() {
   vlib.lcdInitialize(9, 8);
   
  pinMode(10, OUTPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  
  //lcd.begin(16,2);
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Charging and");
  //lcd.setCursor(0,1);
  //lcd.print("Discharging of C");
  //delay(5000);
  
}


void lcdDisplayData()
{
  //lcd.setCursor(0,0);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("T(ms): ");
  vlib.lcdCmd(0x87);
  //lcd.setCursor(7,0);
  char ttimes[20];
  floatToString(ttimes, ttime, 7, 1);
  vlib.lcdPrint(ttimes);
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC4);
  vlib.lcdPrint("Vc(volts): ");
  //lcd.setCursor(10,1);
  vlib.lcdCmd(0xCA);
  char voltss[5];
  floatToString(voltss, volts, 2, 2);
  vlib.lcdPrint(voltss);
}



void loop() {
  //lcd.clear();
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("Flip to Charge");
  delay(5000);
  //lcd.clear();
  //lcd.setCursor(0,0);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Data is being");
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("collected...  ");
  delay(2000);
  //lcd.clear();
  start=millis();
  while (1) {
  sensorValue = analogRead(analogInPin);
  volts=(sensorValue*5.0)/(1023.0);
  ttime= (millis()-start);
  Serial.print(ttime);
  Serial.print("\t");  
  Serial.println(volts,5); 
  lcdDisplayData(); 
  
  }
}


char * floatToString(char * outstr, double val, byte precision, byte widthp) {
  char temp[16]; //increase this if you need more digits than 15
  byte i;

  temp[0] = '\0';
  outstr[0] = '\0';

  if (val < 0.0) {
    strcpy(outstr, "-\0"); //print "-" sign
    val *= -1;
  }

  if ( precision == 0) {
    strcat(outstr, ltoa(round(val), temp, 10)); //prints the int part
  }
  else {
    unsigned long frac, mult = 1;
    byte padding = precision - 1;

    while (precision--)
      mult *= 10;

    val += 0.5 / (float)mult;    // compute rounding factor

    strcat(outstr, ltoa(floor(val), temp, 10)); //prints the integer part without rounding
    strcat(outstr, ".\0"); // print the decimal point

    frac = (val - floor(val)) * mult;

    unsigned long frac1 = frac;

    while (frac1 /= 10)
      padding--;

    while (padding--)
      strcat(outstr, "0\0");   // print padding zeros

    strcat(outstr, ltoa(frac, temp, 10)); // print fraction part
  }

  // generate width space padding
  if ((widthp != 0) && (widthp >= strlen(outstr))) {
    byte J = 0;
    J = widthp - strlen(outstr);

    for (i = 0; i < J; i++) {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp, outstr);
    strcpy(outstr, temp);
  }

  return outstr;
}


