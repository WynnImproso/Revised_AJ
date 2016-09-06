


#include <SPI.h>
#include <vLib.h>
vLib vlib;


void setup() {

  pinMode(5, OUTPUT);

  Serial.begin(115200); // use the serial port
  vlib.lcdInitialize(9, 8); // 9 - DATA; 8 - CLOCK
  vlib.lcdCmd(0x84); // Prints in the first line; 80 to 8F
vlib.lcdPrint("VISSERX");
  
  TIMSK0 = 0; // turn off timer0 for lower jitter - delay() and millis() killed
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  
}

void loop() {
  int prevData=0;
  int newData=0;
  int skip=0;
  boolean ups[256];
  int i=0;
  for (i=0; i<256; i++){
    ups[i]=0;
  }
  
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      
      
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0xf5; // restart adc
      
      if (skip%125==0){
        prevData = newData;//store previous value
      
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      

      
      int k = (j << 8) | m; //  form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
    newData=k;

      }
      skip++;
      
    
  
  if (newData > prevData){//if positive slope
   PORTD |= 0b00100000;
   ups[i/2]=true;
  }
  else if (newData < prevData){
    PORTD &= ~0b00100000;
    ups[i/2]=false;
  }
  
      
      
      

      
    }
       sei(); // turn interrupts back on
    
      

  int max_possible_index=56;
 int max_diff=0;
  int current_max_index=0;
    
    int freq=0;
    int prev=0;
    int next=0;
    int sample=0;
    for(i=0; i<256; i++){
        if (ups[i]==true){
          prev=next;
          next=i;
          if (prev!=0 && next!=0){
            freq= (freq*sample + get_corresponding_f(next-prev))/(sample+1);
            sample++;
          }
            
          
        }
      
    }
    vlib.lcdCmd(0xC4);
    vlib.lcdPrint("  ");
    vlib.lcdPrint(freq);
      vlib.lcdPrint(" Hz        ");
    
    Serial.print(freq);
    Serial.print("Hz");
  }

}

float get_corresponding_f(int diff){
  float fs=2500.0; //as observed

  return (((index) + (index+1))*fs)/(2*FFT_N);
  
}

