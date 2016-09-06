#include <dht.h>
#include <vLib.h>
vLib vlib;

#define dht_dpin 4 //no ; here. Set equal to channel sensor is on

#define DHTTYPE DHT22   


void setup(){
  // set up the LCD's number of columns and rows: 
  // initialize the serial communications:
  Serial.begin(9600);
    vlib.lcdInitialize(9, 8);
    vlib.InitializeHumidityDHT(dht_dpin);
  delay(300);//Let system settle
 // vlib.lcdPrint("Humidity and temperature\n\n");
  
  delay(700);//Wait rest of 1000ms recommended delay before
  //accessing sensor
}//end "setup()"

void loop(){
  //This is the "heart" of the program.
  
 vlib.InitializeHumidityDHT(dht_dpin);
char cms[20];
    vlib.lcdCmd(0x80);
    vlib.lcdPrint(vlib.ReadHumidityDHT());
    vlib.lcdPrint("%");
    vlib.lcdCmd(0xC0);
    delay(1500);
    vlib.lcdPrint("Temp = ");
    floatToString(cms,  vlib.ReadTemperatureDHT(), 1, 3);
    vlib.lcdPrint(cms);
    vlib.lcdPrint("C"); 

  //Don't try to access too frequently... in theory
  //should be once per two seconds, fastest,
  //but seems to work after 0.8 second.
   // when characters arrive over the serial port...
   
  delay(1500);
}// end loop()

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
