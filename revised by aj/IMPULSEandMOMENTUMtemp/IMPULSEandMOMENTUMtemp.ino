/* FSR simple testing sketch. 
 
Connect one end of FSR to power, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground 

 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino pin 7 
 Trig to Arduino pin 8
 
For more information see www.ladyada.net/learn/sensors/fsr.html */

//#include<LiquidCrystal.h>
#include <SD.h>
#include <string.h>
#include <SPI.h>



#include <vLib.h>
vLib vlib;

// Initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);

// this constant won't change.  It's the pin number
// of the sensor's output:
const int pingPin = 5;
const int echoPin =7;
const int chipSelect = 10;
int fsrPin = 0 ;     // the FSR and 10K pulldown are connected to a0
float fsrReading;     // the analog reading from the FSR resistor divider
float fsrReading1; 
int buttonPin = 4;  // button connected to digital pin 6
int buttonState = 0;     // current state of the button
int lastButtonState = 0; // previous state of the button 
 
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(115200);  
  pinMode(buttonPin,INPUT);    // 
  //lcd.begin(16,2);
  //lcd.clear();
  pinMode(10, OUTPUT);

  delay(50);

   vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
 vlib.lcdCmd(0x84);

}
 
void loop(void) {
  
  long t0=-1;
  long t=0;
  float v0=0;
  float v=0;
  float vmax = 0; 
  float vmin = 0;
  float tmax = 0; 
  float tmin = 0;  
  float fsrmax = 0; 
  float cm0 = 0;
  long duration, inches;
  float cm;
  float dV;
  float dt = 0; 
  float reading = 0; 
  int index = 0; 
  float ave = 0;
  float imp = 0; 
  
  while(1){ 
   
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    
     fsrReading1 = analogRead(fsrPin);  
     fsrReading = 1000*0.0162*(exp(1.5793*fsrReading1/1000));
           
          pinMode(pingPin, OUTPUT);
          digitalWrite(pingPin, LOW);
          delayMicroseconds(2);
          digitalWrite(pingPin, HIGH);
          delayMicroseconds(5);
          digitalWrite(pingPin, LOW);
          pinMode(echoPin, INPUT);
          duration = pulseIn(echoPin, HIGH);
          
          cm = microsecondsToCentimeters(duration);
          float cm_current = filter(cm, 0.4, cm0);
          cm = cm_current;
          t = millis();
          v=(cm-cm0)*(10)/(t-t0);
          cm0 = cm;
          t0=t;
             
          if (fsrReading >= 20) { dt += (t - t0); index++; reading += fsrReading; imp += (t - t0)*fsrReading; } else {}   
          if(fsrReading > fsrmax) {fsrmax = fsrReading;}  else {}                             
          Serial.print(t); 
          Serial.print(" ");       
          Serial.print(cm); 
          Serial.print(" ");       
          Serial.println(v); 
          Serial.print(" fsr");
          Serial.println(fsrReading);
          Serial.print("\n orig fsr");
          Serial.println(fsrReading1);
          Serial.print("\n");
          
          if(v > vmax) {vmax = v;}  else {}  
          if(v < vmin) {vmin = v;}  else {}       
          lastButtonState = buttonState;    
          digitalWrite(buttonPin, HIGH);
          delay(5);
           
      }
      
  else {
        if (lastButtonState == buttonState) {digitalWrite(buttonPin, HIGH); delay(1);} 
        
        else {
           ave = reading/index;
           dV = vmax+vmin;  
           Serial.print(dV);
           Serial.print(" ");     
           Serial.print(dt);
           Serial.print(" ");
           //Serial.print(vmin);
           //Serial.print(" ");
           //Serial.print(fsrmax);
           //Serial.print(" ");
           //Serial.print(ave);
           //Serial.print(" ");
           //Serial.println(reading);
           Serial.print(" ");
           Serial.println(imp);
                  
         //lcd.setCursor(0,0);
         vlib.lcdCmd(0x80);
         vlib.lcdPrint("dV:           "); 
         //lcd.setCursor(3,0);
         vlib.lcdCmd(0x83);
         char dvs[4];
         floatToString(dvs, dV, 1, 2);
         //lcd.print(dV);
         vlib.lcdPrint(dvs); 
         vlib.lcdPrint(" cm/s        "); //change me to cm/s
         
         //lcd.setCursor(0,1);
         vlib.lcdCmd(0xC0);
         vlib.lcdPrint("Fmax:           ");
         //lcd.setCursor(5,1);
         vlib.lcdCmd(0xC5);
         //lcd.print(ave);
         char aves[4];
         floatToString(aves, ave, 1, 2);
         vlib.lcdPrint(aves);  
         vlib.lcdPrint(" mN        "); //change me to cm/s
         
         index = 0;    
         reading = 0; 
         t0=-1;
         t=0;
         v0=0;
         v=0;
         vmax = 0; 
         vmin = 0; 
         tmax = 0; 
         tmin = 0; 
         dt = 0; 
         fsrmax = 0; 
         cm0 = 0;
         imp = 0; 
         lastButtonState = buttonState;    
         digitalWrite(buttonPin, HIGH);
         
         delay(5);

               }
          }   
          
  }
  
}


long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

float microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return (1.0*microseconds / 29.0) * 0.5;
}

// filter the current result using a weighted average filter:
float filter(float rawValue, float weight, float lastValue) {
  // run the filter:
  float result = weight * rawValue + (1.0-weight)*lastValue;
  // return the result:
  return result;
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
