/*    
 *  A simple program to output the current temperature to the IDE's debug window 
 * 
 *  
 */
//#include <vLib.h>
//vLib vlib;

//TMP36 Pin Variables
int lightPinPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
                        //the resolution is 10 mV / degree centigrade 
                        //(500 mV offset) to make negative temperatures an option

/*
 * setup() - this function runs once when you turn your Arduino on
 * We initialize the serial connection with the computer
 */
void setup()
{
  Serial.begin(9600);  //Start the serial connection with the copmuter
                       //to view the result open the serial monitor 
                       //last button beneath the file bar (looks like a box with an antenae)
  //vlib.lcdInitialize(9,8); // 9 - DATA; 8 - CLOCK 
  // print to lcd
  //vlib.lcdCmd(0x84); // Prints in the first line; 80 to 8F
                    // 128(1r,1c) to 143(1r,16c)
  //vlib.lcdPrint("VISSERX");
  //vlib.lcdCmd(0xC2); // Prints in the second line; C0 to CF
  //vlib.lcdPrint("Temperature");
}
 
void loop()                     // run over and over again
{
 float resistance = getResistance(0);  //getting the voltage reading from the temperature sensor
                                                  //to degrees ((volatge - 500mV) times 100)
 Serial.println(resistance);                     //printing the result
 delay(1000);                                     //waiting a second
}

/*
 * getVoltage() - returns the voltage on the analog input defined by
 * pin
 */
float getResistance(int pin){
  float Vr=analogRead(pin)* .004882814; 
 return Vr*10000.0/(5.0-Vr); //converting from a 0 to 1024 digital range
                                        // to 0 to 5 volts (each 1 reading equals ~ 5 millivolts
}
