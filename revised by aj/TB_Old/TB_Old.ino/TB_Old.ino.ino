#include <dht.h>
dht DHT;

#define DHT11_PIN 2

#include <SFE_BMP180.h>
#include <Wire.h>

 
#define  hallPin 3  // the pin that the pushbutton is attached to
#define ledPin 13   // the pin that the LED is attached to

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   //


#include "SIM900.h"
#include <SoftwareSerial.h>
//#include "inetGSM.h"
#include"string.h"
#include "sms.h"
SMSGSM sms;
boolean msgsend = false;


String dataconv; 
char hall = 0;
char msg[50];
int numdata;
char inSerial[50];
int i=0;
boolean started=false;
char smsdata [160];
//String dataconv; 
SFE_BMP180 pressure;
double baseline; // baseline pressure
long milli;
// Variables will change:
int hallCounter = 0;   // counter for the number of tip
int * whatevs= &hallCounter;
int hallState = 0;         // current state
int lasthallState = 0;     // previous state
int elapse;
volatile byte time;
volatile byte timeold;
//Timer
#include <SimpleTimer.h>
//SimpleTimer timer;
SimpleTimer timer2;
SimpleTimer timer3;
//SimpleTimer timer4;

/*void  DigitalClockDisplay(){
  int h,m,s;
  s = millis() / 1000;
  m = s / 60;
  h = s / 3600;
  s = s - m * 60;
  m = m - h * 60;
  Serial.print(h);
  printDigits(m);
  printDigits(s);
  Serial.println();
}
*/
int smscounter =0;


void setup() {
  // initialize the  pin as a input:
  // initialize the LED as an output:
Serial.begin(9600);
   Serial.println("GSM Shield testing.");
//     start configuration of shield with baudrate.
    if (gsm.begin(2400)) {
          Serial.println("\nstatus=READY");
          started=true;
     }
     
     
//timer.setInterval((86400000/(24*60))+2, DigitalClockDisplay);
timer2.setInterval(86400000, RepeatTask);//timer of sending
//timer4.setInterval(60000, RepeatTask2);
timer3.setInterval(86400001, hallReset);


   pinMode(hallPin, INPUT);
   pinMode(ledPin, OUTPUT);


  if (pressure.begin())
    Serial.println("BMP180 init success");
   else
   Serial.print("pressure failed");
  baseline = getPressure();

};

//VOID LOOP HERE
void loop() 
{
//timer.run();
timer2.run();
timer3.run();
  hallState = digitalRead(hallPin);
  
  if (hallState != lasthallState) {

    if (hallState == HIGH) {
  
      hallCounter++;
      
      milli=hallCounter*134.1;      
      
      time = millis()/1000; 
  
}
    else {
      // if the current state is LOW then the pin
      // went from on to off:
      hallCounter++;

      milli=hallCounter*134.1;
     
      time = millis()/1000;   
    
//         dataconv = "/welcome.php?a=1234&t=";
          dataconv.concat(hallCounter);         
         // strcpy (link,dataconv.c_str());
          
     elapse = time;
//     charvolume = Volume;     
    }
      }
  // save the current state as the last state, 
  //for next time through the loop
  lasthallState = hallState;
          
  
  if (hallCounter % 4 == 0) {
    digitalWrite(ledPin, HIGH);
  } else {
   digitalWrite(ledPin, LOW);
  }


 double P;
  
  // Get a new pressure reading:
  P = getPressure();
{
 dataconv =  "Rain gauge data, ";
 dataconv.concat(milli);
 dataconv = dataconv + "mm/1000 per 2hr, ";
 dataconv.concat(P);
dataconv.concat("kPa, "); 


 
  }
  
}


void hallReset() {
   hallCounter=0;
   *whatevs=0;
   milli=0;
  
  }


void RepeatTask() {
//   hallCounter=0;
//   *whatevs=0;

   DHT.read11(DHT11_PIN);
float h;
double T;

 h=DHT.humidity;
 dataconv.concat(h);
 dataconv.concat("% humidity, ");
 h=DHT.temperature;
  dataconv.concat(h);
 dataconv.concat("C, ");
  char status = pressure.startTemperature();
  
  if (status != 0)
  {
    // Wait for the measurement to complete:
  
    delay(status);
  }
pressure.getTemperature(T);
dataconv.concat(T);
 dataconv.concat(" C ");


  dataconv.concat("End of text message");
 strcpy (smsdata,dataconv.c_str());
 Serial.println(smsdata);
 
     msgsend = true; if(started){
     if (msgsend) {
       
 if (sms.SendSMS("09263466285",smsdata));


 
  Serial.println("\n SMS sent OK");
//  Serial.println(smsdata);
  }
 }
 
//   hallCounter=0;
 
}




double getPressure()
{
  char status;
  double T,P,p0,a;

  // You must first get a temperature measurement to perform a pressure reading.
  
  // Start a temperature measurement:
  // If request is successful, the number of ms to wait is returned.
  // If request is unsuccessful, 0 is returned.

  status = pressure.startTemperature();
  
  if (status != 0)
  {
    // Wait for the measurement to complete:

    delay(status);

    // Retrieve the completed temperature measurement:
    // Note that the measurement is stored in the variable T.
    // Use '&T' to provide the address of T to the function.
    // Function returns 1 if successful, 0 if failure.

    status = pressure.getTemperature(T);

    if (status != 0)
    {
      // Start a pressure measurement:
      // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      // If request is successful, the number of ms to wait is returned.
      // If request is unsuccessful, 0 is returned.

      status = pressure.startPressure(3);
      if (status != 0)
      {
        // Wait for the measurement to complete:
        delay(status);

        // Retrieve the completed pressure measurement:
        // Note that the measurement is stored in the variable P.
        // Use '&P' to provide the address of P.
        // Note also that the function requires the previous temperature measurement (T).
        // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
        // Function returns 1 if successful, 0 if failure.

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        
      }
      
    }
    
  }

}
