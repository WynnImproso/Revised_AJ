#include <SPI.h>

#include <vLib.h>
vLib vlib;
/* Ping))) Sensor

   This sketch reads a PING))) ultrasonic rangefinder and returns the
   distance to the closest object in range. To do this, it sends a pulse
   to the sensor to initiate a reading, then listens for a pulse
   to return.  The length of the returning pulse is proportional to
   the distance of the object from the sensor.

   The circuit:
    * +V connection of the PING))) attached to +5V
    * GND connection of the PING))) attached to ground
    * SIG connection of the PING))) attached to digital pin 7

   http://www.arduino.cc/en/Tutorial/Ping

   created 3 Nov 2008
   by David A. Mellis
   modified 30 Aug 2011
   by Tom Igoe

   This example code is in the public domain.

 */
//#include<LiquidCrystal.h>

#include <string.h>

const int pingPin = 5;
const int echoPin = 7;

const int chipSelect = 10;

int buttonPin = 4
;  // button connected to digital pin 5
int buttonState = 0;     // current state of the button
int lastButtonState = HIGH; // previous state of the button

int log_no = 1;






void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  vlib.lcdCmd(0x84); // Prints in the first line; 80 to 8F
  // 128(1r,1c) to 143(1r,16c)
  vlib.lcdPrint("VISSERX");

  pinMode(buttonPin, INPUT);
  pinMode(10, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pingPin, OUTPUT);


  long t0 = -1;
  long t = 0;
  float v0 = 0;
  float v = 0;
  float a = 0;
  float cm0 = 0;
}



void loop()
{
  long t0 = -1;
  long t = 0;
  float v0 = 0;
  float v = 0;
  float vi=0;
  float a = 0;
  float cm0 = 0;

  int reading_counter = 0;

  while (1) {

    long duration, inches;
    float cm;

    //digitalWrite(pingPin, LOW);
    //because I'm a freaking genius
    PORTD &= ~0b00100000;
    delayMicroseconds(400);
    //digitalWrite(pingPin, HIGH);

    PORTD |= 0b00100000;
    delayMicroseconds(1000);
    //digitalWrite(pingPin, LOW);
    PORTD &= ~0b00100000;

    duration = pulseIn(echoPin, HIGH);

    cm0 = cm;
    cm = microsecondsToCentimeters(duration);
    t0 = t;
    t = millis();
    v0 = v;
    v = (cm - cm0) * (10) / (t - t0);
    a = (v - v0) * 1000 / (t - t0);





    buttonState = digitalRead(buttonPin);
    if (buttonState == LOW) {
      vi=v;
      vlib.lcdCmd(0x84);
      char vis[5];
       floatToString(vis, vi, 1, 2);
       vlib.lcdPrint(vis);
       vlib.lcdPrint(" m/s        ");
    }
   
 
      char cms[5];
      floatToString(cms, cm, 1, 3);
      vlib.lcdCmd(0xC4);
      vlib.lcdPrint(cms);
      vlib.lcdPrint(" cm        ");
     

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
  return (1.0 * microseconds / 29.0) * 0.5;
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
