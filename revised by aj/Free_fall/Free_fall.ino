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

#include <SD.h>
#include <string.h>

// Initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);
// this constant won't change.  It's the pin number
// of the sensor's output:
const int pingPin = 5;
const int echoPin = 7;

const int chipSelect = 10;

int buttonPin = 4;  // button connected to digital pin 5
int buttonState = 0;     // current state of the button
int lastButtonState = HIGH; // previous state of the button
long readings_t[30];
float readings_cm[30];
File dataFile;

int log_no = 1;






void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  vlib.lcdCmd(0x84); // Prints in the first line; 80 to 8F
  // 128(1r,1c) to 143(1r,16c)
  vlib.lcdPrint("VISSERX");

  //lcd.begin(16,2);
  //lcd.clear();

  pinMode(buttonPin, INPUT);
  pinMode(10, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pingPin, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized." + String(3));
  bool b = true;
  while (b) {
    String a = "data" + String(log_no) + ".csv";
    char fileNameCharArray[a.length() + 1];
    a.toCharArray(fileNameCharArray, a.length() + 1);
    bool c = SD.exists(fileNameCharArray);
    if (c) {
      Serial.println(fileNameCharArray);
      log_no = log_no + 1;
    } else {
      Serial.println("filename");
      b = false;
    }
  }
  Serial.println("Writing at: data" + String(log_no) + ".csv");

  reset_readings();

  long t0 = -1;
  long t = 0;
  float v0 = 0;
  float v = 0;
  float a = 0;
  float cm0 = 0;
}

void reset_readings() {
  int i = 0;
  for (i = 0; i < 25; i++) {
    readings_t[i] = -1;
    readings_cm[i] = -1;
  }
}

void loop()
{
  long t0 = -1;
  long t = 0;
  float v0 = 0;
  float v = 0;
  float a = 0;
  float cm0 = 0;

  int reading_counter = 0;

  while (1) {
    // establish variables for duration of the ping,
    // and the distance result in inches and centimeters:
    long duration, inches;
    float cm;


    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

    //digitalWrite(pingPin, LOW);
    //because I'm a freaking genius
    PORTD &= ~0b00100000;
    delayMicroseconds(400);
    //digitalWrite(pingPin, HIGH);

    PORTD |= 0b00100000;
    delayMicroseconds(1000);
    //digitalWrite(pingPin, LOW);
    PORTD &= ~0b00100000;

    // The same pin is used to read the signal from the PING))): a HIGH
    // pulse whose duration is the time (in microseconds) from the sending
    // of the ping to the reception of its echo off of an object.

    duration = pulseIn(echoPin, HIGH);


    // convert the time into a distance
    // inches = microsecondsToInches(duration);
    cm0 = cm;
    cm = microsecondsToCentimeters(duration);
    t0 = t;
    t = millis();
    v0 = v;
    v = (cm - cm0) * (10) / (t - t0);
    a = (v - v0) * 1000 / (t - t0);



    //Serial.println("Distance:");
    //Serial.print(inches);
    //Serial.print("in, ");
    //Serial.print(cm);
    //Serial.println(" cm");
    //Serial.println();
    //Serial.println();

    //Serial.println("Velocity:");
    //Serial.println(v);


    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.

    /*
     // make a string for assembling the data to log:
     String dataString = "";
     // if the file is available, write to it:
     char vs[20];
     floatToString(vs, v, 3, 3);
     char as[20];
     floatToString(as, a, 3, 3);
     char cms[20];
     floatToString(cms, cm, 3, 3);

     dataString+= t;
     dataString+=",";

     dataString+=cms;
     dataString+=",";

     dataString+=vs;
     dataString+=",";

     dataString+=as;
     dataString+=",";
    */

    buttonState = digitalRead(buttonPin);

    if (buttonState == LOW) {
      //      Serial.println("Button: Low");
      lastButtonState = LOW;
      if (reading_counter < 25) {
        readings_t[reading_counter] = t;
        readings_cm[reading_counter] = cm;
        reading_counter += 1;

      }
    }
    else {
      //lcd.print("Distance:           ");
      //lcd.setCursor(0,1);
      vlib.lcdCmd(0x84); //whatever this means

     char cms[20];
     floatToString(cms, cm, 1, 3);
     
      vlib.lcdPrint(cms);
      vlib.lcdPrint(" cm        ");
      //    Serial.println("Button: High");
      if (lastButtonState == LOW) {





        //    Serial.println("numbers of readings: ");
        //   Serial.println(reading_counter);

        writeReadings();
        log_no = log_no + 1;

        //only display at the end of shiz
        //Serial.println("Acceleration:");
        //Serial.println(avea());
        //char as[20];
        //floatToString(as, avea(), 3, 3);
        //Serial.print(as);
        vlib.lcdCmd(0xC4);
        vlib.lcdPrint("a :");
        char as[4];
        floatToString(as, avea(), 1, 2);
        
        vlib.lcdPrint(as);
        vlib.lcdPrint("     ");
        reset_readings();
        reading_counter = 0;
        lastButtonState = HIGH;

      }

    }

    // if the file isn't open, pop up an error:
    delayMicroseconds(16000);
    //delay(8);
  }
}

float avea() {
  int i = 0;

  long t0 = -1;
  long t = 0;
  float v0 = 0;
  float v = 0;
  float a = 0;
  float cm0 = 0;
  float cm = 0;

  int points = 0;
  float output = 0.0;

  for (i = 0; i < 25; i++) {
    if (readings_t[i] != -1) {
      cm0 = cm;
      cm = readings_cm[i];
      t0 = t;
      t = readings_t[i];
      v0 = v;
      v = (cm - cm0) * (10) / (t - t0);
      a = (v - v0) * 1000 / (t - t0);
      Serial.println(a);
      
        output = ((output * points) + a) / (points + 1);
        points++;
    }
  }
  return output;
}

void writeReadings()
{


  int i = 0;

  long t0 = -1;
  long t = 0;
  float v0 = 0;
  float v = 0;
  float a = 0;
  float cm0 = 0;
  float cm = 0;
  for (i = 0; i < 25; i++) {
    if (readings_t[i] != -1) {
      //write

      cm0 = cm;
      cm = readings_cm[i];
      t0 = t;
      t = readings_t[i];
      v0 = v;
      v = (cm - cm0) * (10) / (t - t0);
      a = (v - v0) * 1000 / (t - t0);
      String dataString = "";
      // if the file is available, write to it:
      char vs[20];
      floatToString(vs, v, 3, 3);
      char as[20];
      floatToString(as, a, 3, 3);
      char cms[20];
      floatToString(cms, cm, 3, 3);

      dataString += t;
      dataString += ",";

      dataString += cms;
      dataString += ",";

      dataString += vs;
      dataString += ",";

      //dataString+=as;
      //dataString+=",";


      String hell = "data" + String(log_no) + ".csv";
      char fileNameCharArray[hell.length() + 1];
      hell.toCharArray(fileNameCharArray, hell.length() + 1);
      dataFile = SD.open(fileNameCharArray, FILE_WRITE);
      if (dataFile) {
        dataFile.println(dataString);
        dataFile.close();
      }
      else {

        Serial.println("Writing failed");
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
