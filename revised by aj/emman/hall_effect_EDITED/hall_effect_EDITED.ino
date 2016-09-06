/*
 * Hall Effect Probe code
 * January 6, 2015
 * Edited by: Elisha Jeune Hidalgo and Charlene Jalosjos
 * Program from: hall_probe_LED by Altoveros and Ramirez
 */

#include <vLib.h>
vLib vlib;

int count = 30;  //1min and 30seconds
const float m=12.706;  //slope
const float b=-31.366; //intercept
const int analogInPin1 = A1;  // Analog input pin that the potentiometer is attached to
//const int analogInPin2 = A2;  // Analog input pin that the potentiometer is attached to
const int PB = 1;             //push button
const int LED = 3;           //buzzer
int n=50;          //50 samples

int sensorValue1 = 0;    //preset value
int sensorValue2 = 0;    //continuous readout 
long int start;
float volts1=0.0;
float volts2=0.0;
float tesla1=0.0;
float tesla2=0.0;
long ttime=0;
float diff=0.01;

void setup() {
  Serial.begin(9600);
  vlib.lcdInitialize(9,8); // 9 - DATA; 8 - CLOCK
  welcome();  //displays welcome message at start/reset
  warmup();
 // m = millis();
}

void loop() {
   while(digitalRead(PB)==HIGH) {}
//    lcd.clear();
    start=millis();
    sensorValue1=0.0;
    for (int i=0;i<n;i++) {
     sensorValue1 += analogRead(analogInPin1);
  }
    volts1=(sensorValue1*5.0)/(n*1023.0);
  tesla1=m*volts1+b;
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
  if (abs(volts1-volts2)<=diff) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED,LOW);
    
  }
  Serial.print(tesla1,5);
  Serial.print("\t");  
  Serial.println(tesla2,5); 
 // lcdDisplayData(); 
  delay(500);
  }

}

void seriallcd(){
  vlib.lcdCmd(0x80); // Prints in the first line  
  vlib.lcdPrint("Hall:");
  vlib.lcdPrint(int(co2));
}

void welcome() {
  vlib.lcdCmd(0x85);
  vlib.lcdPrint("VISSER");
  vlib.lcdCmd(0xC4);
  vlib.lcdPrint("Handheld");
  delay(2000);
//  vlib.lcdClear();
  //vlib.lcdCmd(0xC6);
  //vlib.lcdPrint("Hall Effect Probe");
 // vlib.lcdCmd(0xC5);
  //vlib.lcdPrint("Sensor");
  delay(2000);
//  vlib.lcdClear();
}

void warmup() {
  while (count != 0) {
    vlib.lcdCmd(0x80);
    vlib.lcdPrint("Warming up...");
    vlib.lcdCmd(0xC0);
    vlib.lcdPrint(count--);
    vlib.lcdPrint("sec left   ");
    delay(1000);
    //vlib.lcdClear();
  } 
}
