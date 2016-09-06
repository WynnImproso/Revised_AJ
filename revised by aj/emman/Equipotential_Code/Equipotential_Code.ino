int vin = 0; //initial value
int vref = 0; //initial value
const int ledpin = 6;
const int button = 5;
const int power = 4;
int buttonState = 0;

void setup() {
  Serial.begin(9600); //serial monitor for testing
  pinMode(ledpin, OUTPUT); 
  pinMode(button, INPUT);
  pinMode(power, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(power, HIGH);
  buttonState = digitalRead(button);
  
  vin=analogRead(A4);
  Serial.print("\n");
  if (buttonState == HIGH){
    vref = vin;
  }

  if ((vin-vref)>0.001){
    digitalWrite(ledpin, LOW);
  }else if ((vin-vref)<0.001&(vin-vref)>-0.001){
    digitalWrite(ledpin, HIGH);
  }

  Serial.print("\nFixed Probe: ");
  Serial.print(((5.0*vref)/1024.0), 3);
  Serial.print("\nMovable Probe: ");
  Serial.print(((5.0*vin)/1024.0), 3);
  

  delay(1000);
}
