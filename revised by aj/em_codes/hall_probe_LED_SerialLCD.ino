//program for magnetic equipotential line plotting
// data can be stored in sd card
//programmed by nelio c. altoveros and hazel joyce m.ramirez
//october 7, 2013

//#include <SPI.h>
//#include <SD.h>
#include <stdio.h>
//#include<LiquidCrystal.h>

#include <vLib.h>
#include <stdlib.h>
vLib vlib;

// Initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);

const float m=12.706;  //slope
const float b=-31.366; //intercept
const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to
//const int analogInPin2 = A2;  // Analog input pin that the potentiometer is attached to
const int PB = 4;             //push button
const int LED = 5;           //buzzer
int n=20;                    //50 samples

long int sensorValue1 = 0;    //preset value
long int sensorValue2 = 0;    //continuous readout 
long int start;
float volts1=0.0;
float volts2=0.0;
float tesla1=0.0;
float tesla2=0.0;
long ttime=0;
float diff=0.01;


void setup() {
  pinMode(10, OUTPUT);
  pinMode (LED, OUTPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  analogReference(INTERNAL);
  vlib.lcdInitialize(9,8);
  
  //lcd.begin(16,2);
  //vlib.lcdClear();
  vlib.lcdCmd(0x80); // Prints in the first line
  vlib.lcdSetCursor(0,0);
  vlib.lcdPrint("Magnetic Field");
  vlib.lcdCmd(0xCD); // Prints in the second line
  vlib.lcdSetCursor(0,1);
  vlib.lcdPrint("E Plotting");
  delay(5000);
  
}


void lcdDisplayData()
{
  //vlib.lcdClear();
  //vlib.lcdGoToFirstLine();
  vlib.lcdCmd(0x80); // Prints in the first line
  vlib.lcdSetCursor(1,1);
  vlib.lcdPrint("SB(mT): ");
  if (tesla1>=0.00)
  vlib.lcdSetCursor(1,10);
  else
  vlib.lcdSetCursor(1,9);  
  //vlib.lcdPrint(tesla1,2);
  vlib.lcdPrint(tesla1);
  if (abs(tesla1)>=10.00)
    vlib.lcdSetCursor(1,15);
  else
    vlib.lcdSetCursor(1,14);
  vlib.lcdPrint("    ");
  
  vlib.lcdCmd(0xCD); // Prints in the second line
  //vlib.lcdGoToSecondLine();
  vlib.lcdSetCursor(2,1);
  vlib.lcdPrint("B(mT): ");
  if (tesla2>=0.00)
    vlib.lcdSetCursor(2,10);
  else
    vlib.lcdSetCursor(2,9);  
  //vlib.lcdPrint(tesla2,2);
  vlib.lcdPrint(tesla2);
  if (abs(tesla2)>=10.00)
    vlib.lcdSetCursor(2,15);
  else
    vlib.lcdSetCursor(2,14);
  vlib.lcdPrint("    ");
  
}



void loop() {
  //vlib.lcdClear();
  vlib.lcdCmd(0x80); // Prints in the first line
  //vlib.lcdGoToFirstLine();
  vlib.lcdSetCursor(1,1);
  vlib.lcdPrint("Place probe on ");
  vlib.lcdCmd(0xCD); // Prints in the second line
  //vlib.lcdGoToSecondLine();
  vlib.lcdSetCursor(2,1);
  vlib.lcdPrint("E Plotting Board");  
  delay(5000);
  //vlib.lcdClear();
  vlib.lcdCmd(0x80); // Prints in the first line
  //vlib.lcdGoToFirstLine();
  vlib.lcdSetCursor(1,1);
  vlib.lcdPrint("Press button");
  vlib.lcdCmd(0xCD); // Prints in the second line
  //vlib.lcdGoToSecondLine();
  vlib.lcdSetCursor(2,1);
  vlib.lcdPrint("to store value");
  delay(5000); 

  while(digitalRead(PB)==HIGH) {}
    //vlib.lcdClear();
    start=millis();
    sensorValue2=0.0;
    for (int i=0;i<n;i++) {
     sensorValue2 += analogRead(analogInPin1);
  }
    volts2=(sensorValue2*5.0)/(n*1023.0);
  tesla2=m*volts2+b;
  tesla1=tesla2;
  while (1) {
    if (digitalRead(PB)==LOW) {
      sensorValue1=0.0;      
      for (int i=0;i<n;i++) {
        sensorValue1 += analogRead(analogInPin1);
      } 
    volts1=(sensorValue1*5.0)/(n*1023.0);
    tesla1=m*volts1+b;
    }
    ttime= (millis()-start);
    sensorValue2=0.0;
    for (int i=0;i<n;i++) {
      sensorValue2 += analogRead(analogInPin1);
    }
    volts2=(sensorValue2*5.0)/(n*1023.0);
    tesla2=m*volts2+b;
    ttime= (millis()-start);
    if (abs(tesla1-tesla2)<=diff) {
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED,LOW);
  }
  
  Serial.print(tesla1,5);
  Serial.print("\t");  
  Serial.println(tesla2,5); 
  lcdDisplayData(); 
  delay(500);
  }
}


