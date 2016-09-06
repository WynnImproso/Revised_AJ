


#include <SPI.h>
#include <vLib.h>
vLib vlib;

/*
fft_adc.pde
guest openmusiclabs.com 8.18.12
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

// do #defines BEFORE #includes
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library
  char copy[256];


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

  int i=0;
  
  for(int i=0; i<256; i++){
    copy[i]=0;
  }
  
}

void loop() {
  
  
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    int skip=0;
    for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
      
      skip++;
      
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      PORTD |= 0b00100000;
      PORTD &= ~0b00100000;
      
      ADCSRA = 0xf5; // restart adc
      if (skip%50==0){
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      

      
      int k = (j << 8) | m; //  form into an int
      k -= 0x0200; // form into a signed int
      k <<= 6; // form into a 16b signed int
      fft_input[i] = k; // put real data into even bins
      fft_input[i+1] = 0; // set odd bins to 0
      }
    }
       sei(); // turn interrupts back on
    
      
    // window data, then reorder, then run, then take output
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
    
  
    //fft_mag_lin8();



  int max_possible_index=56;
 int max_diff=0;
  int current_max_index=0;

  int i=2;
  for (i=2;i<max_possible_index; i++){
    if(fft_log_out[i]-copy[i]>=max_diff){
      max_diff=fft_log_out[i]-copy[i];
     current_max_index=i;
  }
   
  }

    int j;
    for (j=0; j<256;j++){
      copy[j]=fft_log_out[j];
    }
  
    
   int freq=get_corresponding_f(current_max_index);

   

 
    
    
    vlib.lcdCmd(0xC4);
    vlib.lcdPrint("  ");
    vlib.lcdPrint(freq);
      vlib.lcdPrint(" Hz        ");
    
    //Serial.write(255); // send a start byte
    //Serial.write(fft_log_out, 128); // send out the data
    Serial.print(freq);
    Serial.print("Hz");
  }

}

float get_corresponding_f(int index){
  float fs=2500.0; //as observed

  return (((index) + (index+1))*fs)/(2*FFT_N);
  
}

