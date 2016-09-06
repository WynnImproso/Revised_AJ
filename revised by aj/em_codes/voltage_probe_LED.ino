//program for electrostatic equipotential line plotting
// data can be stored in sd card
//programmed by nelio c. altoveros and hazel joyce m.ramirez
//march 7, 2014

//#include <SPI.h>
#include <vLib.h>
vLib vlib;
//#include <SD.h>
//#include <stdio.h>
//#include<LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);

//const float m=12.706;  //slope
//const float b=-31.366; //intercept
const int analogInPin1 = A1;  // Analog input pin that the potentiometer is attached to
//const int analogInPin2 = A2;  // Analog input pin that the potentiometer is attached to
//const 
int PB = 5;             //push button
const int LED = 6;           //buzzer
int n=50;          //50 samples

long int sensorValue1 = 0;    //preset value
long int sensorValue2 = 0;    //continuous readout 
long int start;
float volts1=0.0;
float volts2=0.0;
//float tesla1=0.0;
//float tesla2=0.0;
long ttime=0;
float diff=0.01;
const int power = 4;

void setup() {
  pinMode(power, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode (LED, OUTPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  vlib.lcdCmd(0x80); // Prints in the first line; 80 to 8F
  //lcd.begin(16,2);
  //lcd.clear();
  //lcd.setCursor(0,0);
  vlib.lcdPrint("Electrostatic");
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("Field Plotting");
  delay(2000);
  
}


void lcdDisplayData()
{
  //lcd.clear();
  vlib.lcdCmd(0x80);
  //lcd.setCursor(0,0);
  vlib.lcdPrint("SV(volts): ");
  if (volts1>=0.00)
  //lcd.setCursor(11,0);
  vlib.lcdCmd(0x8B);
  else
  vlib.lcdCmd(0x8A);
  //lcd.setCursor(10,0); 
  char volts1s[7];
  floatToString(volts1s, volts1, 3, 3);
  vlib.lcdPrint(volts1s);
  if (abs(volts1)>=10.00)
    vlib.lcdCmd(0x8F);
    //lcd.setCursor(16,0);
  else
    //lcd.setCursor(15,0);
    vlib.lcdCmd(0x8E);
  vlib.lcdPrint("    ");
  
  
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint(" V(volts): ");
  if (volts2>=0.00)
    //lcd.setCursor(11,1);
    vlib.lcdCmd(0xCB);
  else
    vlib.lcdCmd(0xCA);
    //lcd.setCursor(10,1); 
    char volts2s[7];
    floatToString(volts2s, volts2, 3, 3); 
  vlib.lcdPrint(volts2);
  if (abs(volts2)>=10.00)
    vlib.lcdCmd(0xCF);
    //lcd.setCursor(16,1);
  else
    //lcd.setCursor(15,1);
    vlib.lcdCmd(0xCE);
  vlib.lcdPrint("    ");
  
}



void loop() {
  digitalWrite(power, HIGH);
  //lcd.clear();
  vlib.lcdCmd(0x80);
  //lcd.setCursor(0,0);
  vlib.lcdPrint("Place probe on ");
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("E Plotting Board");  
  delay(5000);
  //lcd.clear();
  vlib.lcdCmd(0x80);
  //lcd.setCursor(0,0);
  vlib.lcdPrint("Press button");
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("to store value");
  delay(5000); 

  while(digitalRead(PB)==HIGH) {}
    //lcd.clear();
    start=millis();
    sensorValue2=0.0;
    for (int i=0;i<n;i++) {
     sensorValue2 += analogRead(analogInPin1);
  }
    volts2=(sensorValue2*5.0)/(n*1023.0);
    volts1=volts2;
  //tesla1=m*volts1+b;
  while (1) {
  if (digitalRead(PB)==LOW) {
    sensorValue2=0.0;
     for (int i=0;i<n;i++) {
     sensorValue2 += analogRead(analogInPin1);
     } 
     volts2=(sensorValue2*5.0)/(n*1023.0);
     //tesla1=m*volts1+b;
  }
  ttime= (millis()-start);
  sensorValue2=0.0;
  for (int i=0;i<n;i++) {
     sensorValue2 += analogRead(analogInPin1);
  }
  volts2=(sensorValue2*5.0)/(n*1023.0);
  //tesla2=m*volts2+b;
  ttime= (millis()-start);
  if (abs(volts1-volts2)<=diff) {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED,LOW);
    
  }
  Serial.print(volts1,5);
  Serial.print("\t");  
  Serial.println(volts2,5); 
  lcdDisplayData(); 
  delay(500);
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


