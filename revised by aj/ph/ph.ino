// Display temperature in LCD using LM35 temperature sensor
#include<LiquidCrystal.h>
// Initialize the library with the numbers of the interface pins
#include <vLib.h>
vLib vlib;
unsigned long time;
//LiquidCrystal lcd(7, 8, 9, 2, 4, 6);

void setup() {
  Serial.begin(9600);
  //lcd.begin(16,2);
  vlib.lcdInitialize(9, 8);
}

void loop() {
  int sensorf, sensorf1, sensorf2, i;
  int sensor1[200];

  double ave;

  //lcd.setCursor(0,0);
  time = millis();
  time = time / 1000;
  Serial.println(time);

  //take five (5) readings from A0 A1 and A2 and get average for data processing
  for (i = 0; i < 200 ; i++) {
    sensor1[i] = analogRead(0);
     delay(5);
  }
  ave = 0.0;

  for (i = 0; i < 200; i++) {
    ave = ((i * ave) + sensor1[i]) / (i + 1);
  }

  //  int temp = ave * 0.09765625; // Convert to C
  double temp1 = (ave * 5.0) / 1024;
  //  lcd.print("temp: ");
  //  lcd.print(temp);
  Serial.print(temp1);
  Serial.print("\t");
  //  int temp1 = ave1;
  // float temp1 = ((((ave1 * 5 )/1024)-0.881)*100)-0.6;
  //  Serial.println(temp1);
  //  lcd.print("temp: ");
  //  lcd.print(temp1);
  vlib.lcdCmd(0x80);
  vlib.lcdPrint("ph:");
  char cms[20];
  floatToString(cms, temp1, 1, 3);
  vlib.lcdPrint(cms);
  //vlib.lcdPrint("C");


  //delay(1000);

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
