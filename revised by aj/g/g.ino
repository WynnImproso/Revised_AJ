#include <TinyWireM.h>
#include <SoftwareSerial.h>

#define rxPin 3
#define txPin 4

#include <tinyBMP085.h>
tinyBMP085 bmp;

SoftwareSerial mySerial(rxPin, txPin);


void setup() {
  // put your setup code here, to run once:
bmp.begin();
 mySerial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  int pres = bmp.readPressure();
  mySerial.print("dafu:");
  
  mySerial.print(pres);
  mySerial.println();

  


  delay(500);
   
  
}
