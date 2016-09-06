#include <dht.h>
#include <vLib.h>
vLib vlib;





#define DHT11_PIN 2

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


{



 
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

   vlib.DHT.read11(DHT11_PIN);
float h;
double T;

 h=DHT.humidity;
 dataconv.concat(h);
 dataconv.concat("% humidity, ");
 h=DHT.temperature;
  dataconv.concat(h);
 dataconv.concat("C, ");



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





