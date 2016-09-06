#include <dht.h>
#include <vLib.h>

//program for Electromagnetically-induced current
//data can be stored in sd card
//programmed by nelio c. altoveros and hazel joyce m.ramirez
//march 25, 2014
vLib vlib;

#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include<LiquidCrystal.h>

// Initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 2, 4, 6);

const int analogInPin1 = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = A1;  // Analog input pin that the potentiometer is attached to
const int chipSelect = 10;    // SDCard Chip Select ***** pin 4 is used by LCD
const float vthresh=0.01;
const int PB =5;  

int suffix=1;
char fname[15]="AMP",filename[15]="AMP001.TXT";
//float volts=0.0;             //sensor reading
long ttime=0;                //elapsed time
String dataString = "";
float v1,v2,vdiff,vmax;

void setup() {
  pinMode(10, OUTPUT);
  pinMode(PB,INPUT);
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  initSDCard();  
  initLcdDisplay();
  delay(9000);
}

void initSDCard()
{
  Serial.print("");
  Serial.println("Magnetically-Induced Current");
  Serial.println("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
   if (!SD.begin(chipSelect)) {
     Serial.println("Card failed, or not present");
     // don't do anything more:
   }
   else {
   Serial.println("Card initialized.");
   }
   delay(2000);
}


void initLcdDisplay()
{
  vlib.lcdInitialize(9, 8);
  //lcd.begin(16,2);
  vlib.lcdCmd(0x84);
  //lcd.clear();
  //lcd.setCursor(0,0);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Induced Current");
  delay(2000);
  //lcd.clear();
  //lcd.setCursor(0,0);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("Press button");
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("to clear Imax");  
}


void lcdDisplayData()
{
  //lcd.clear();
  //lcd.setCursor(0,0);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("I   (mA):         ");
  vlib.lcdCmd(0x8C);
  //lcd.setCursor(12,0);
  
  if (v1>=10) {
    vlib.lcdCmd(0x8B);
  }
  if (v1>=100) {
    vlib.lcdCmd(0x8A);
  }
  vlib.lcdPrint(v1);
  //lcd.print(v1,2);
  //lcd.setCursor(16,0);
  //lcd.print("      ");
  
  
  //lcd.setCursor(0,1);
  vlib.lcdCmd(0xC0);
  vlib.lcdPrint("Imax(mA):         ");
  vlib.lcdCmd(0xCC);
  //lcd.setCursor(12,1); 
 
  if (vmax>=10) {
    vlib.lcdCmd(0xCB);
    //lcd.setCursor(11,1);
  }
  if (vmax>=100) {
    vlib.lcdCmd(0xCA);
    //lcd.setCursor(10,1);
  }
  vlib.lcdCmd(0xCA);
  vlib.lcdPrint(vmax);
  //lcd.setCursor(16,1);
  //lcd.print("      "); 
  
}



void labelData()
{
  while (SD.exists(filename)) {   
      Serial.print(filename);
      Serial.println(" exists");
      snprintf(filename, 12, "%s%03d.TXT", fname,++suffix);
  }
  Serial.print("Saving data to ");
  Serial.println(filename);
  File dataFile = SD.open(filename, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print("TIME");
    dataFile.print('\t');
    dataFile.println("CURRENT");
    dataFile.print("(ms)");
    dataFile.print('\t');
    dataFile.println("(mA)");
    dataFile.print('\t');
    dataFile.println("(mA)");    
    dataFile.close();
  }
    //if the file isn't open, pop up an error:
  else {
    Serial.print("Error opening ");
    Serial.println(filename);
  } 
}


void logData(){
  File dataFile = SD.open(filename, FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    
    dataFile.println(dataString);
    dataFile.close();
  }  
}   



void loop() {
  //int sensorValue = 0;         // value read from the sensor
  long int start;              //start of sampling
  float vtemp=0.0;
  vmax=0.0;
  //labelData();
  Serial.print("TIME(ms)");
  Serial.print('\t'); 
  Serial.print("I(mA)"); 
  Serial.print('\t'); 
  Serial.println("Imax(mA)");  
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
    
    //vtemp=vdiff;
    //vdiff=v1+v2;
    //vdiff=v1;
    ttime= (millis()-start);
    Serial.print(ttime);
    Serial.print("\t\t");  
    Serial.print(v1,2);
    Serial.print("\t");  
    Serial.println(vmax,2);
    dataString=String(ttime)+"\t"+String(v1,2)+"\t"+String(vmax,2);
    if (digitalRead(PB)==HIGH) {
      vmax=0.0;
    }    
 
    if (v1>vmax) {
      vmax=v1;
    }
    v1=analogRead(analogInPin1);
    v2=analogRead(analogInPin2);
    delay(1000);
    v1=100.0*(v1*5.0)/(1023.0);
    v2=100.0*(v2*5.0)/(1023.0);
    
    lcdDisplayData();
    
    logData();
  
  }
}

