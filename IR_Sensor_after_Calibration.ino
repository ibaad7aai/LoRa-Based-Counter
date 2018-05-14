#define debugSerial SerialUSB

int IRpin = 0;                                    // analog pin for reading the IR sensor

void setup() {
  debugSerial.begin(9600);                             // start the serial port
}

void loop() {
 

 //reads a value from the specified pin and maps input voltage from 0 to 3.3v into iteger values of 0 to 1023 
  int SensorValue  = analogRead(IRpin); 

  // converts the value read from specified pin back to voltage  
  float Volts = SensorValue * (3.3/1023); 
  
  // the equation derived from calibration (y = 19.789x^-1.539) where y = distance and x = volts
  delay(10);
  double Distance =  19.798* pow(Volts, -1.539) ;

  debugSerial.print("sensor1:  "); 
  debugSerial.println(Distance);   // print the distance in the serial monitor

  delay(100);                     
}



