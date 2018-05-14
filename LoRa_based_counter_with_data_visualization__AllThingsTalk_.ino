// Uncomment your selected method for sending data
#define CONTAINERS
//#define CBOR
//#define BINARY

// Select your hardware

#define SODAQ_ONE

#include <ATT_LoRaWAN.h>
#include "keys.h"
#include <MicrochipLoRaModem.h>

#define SERIAL_BAUD 57600

#ifdef SODAQ_ONE
  #define debugSerial SerialUSB
  #define loraSerial Serial2
#endif

MicrochipLoRaModem modem(&loraSerial, &debugSerial);
ATTDevice device(&modem, &debugSerial, false, 7000);  // minimum time between 2 messages set at 7000 milliseconds

#ifdef CONTAINERS
  #include <Container.h>
  Container container(device);
#endif

#ifdef CBOR
  #include <CborBuilder.h>
  CborBuilder payload(device);
#endif

#ifdef BINARY
  #include <PayloadBuilder.h>
  PayloadBuilder payload(device);
#endif


#define debugSerial SerialUSB
#define loraSerial Serial2

#define X 90
#define Y 20

/* States */
#define CLEAR 0
#define SENSOR_1_TRIGGERED 1
#define SENSOR_2_TRIGGERED 2
#define BLOCKED 3

#define change 11
#define change_opp 12



const int SENSOR1_TRIGPIN = 2;
const int SENSOR1_ECHOPIN = 3;
const int SENSOR2_TRIGPIN = 4;
const int SENSOR2_ECHOPIN = 5;
const int SENSOR3_TRIGPIN = 9;
const int SENSOR3_ECHOPIN = 10;
const int SENSOR4_TRIGPIN = 11;
const int SENSOR4_ECHOPIN = 12;

int direction1Counter = 0;
int direction2Counter = 0;

int CurrentState = CLEAR;
int People = 0;
int state = 0;

double Sensordistance(int trigPin,int echoPin);




// LED 7  segment Display
int digit1 = 1; //PWM Display pin 1
int digit2 = 13; //PWM Display pin 2
int digit3 = 8; //PWM Display pin 6
int digit4 = 6; //PWM Display pin 8

int segA = 0; //Display pin 14
int segB = A0; //Display pin 16
int segC = A1; //Display pin 13
int segD = A2; //Display pin 3
int segE = A3; //Display pin 5
int segF = A4; //Display pin 11
int segG = A5; //Display pin 15

void setup() {    
  
//LCD SEGMENTS    
  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);

while ((!debugSerial) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open 
    }
pinMode(SENSOR1_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR1_ECHOPIN, INPUT); // Sets the echoPin as an Input
pinMode(SENSOR2_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR2_ECHOPIN, INPUT); // Sets the echoPin as an Input
pinMode(SENSOR3_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR3_ECHOPIN, INPUT); // Sets the echoPin as an Input
pinMode(SENSOR4_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR4_ECHOPIN, INPUT); // Sets the echoPin as an Input

  debugSerial.begin(SERIAL_BAUD);
  while((!debugSerial) && (millis()) < 10000){}  // wait until the serial bus is available
  
  loraSerial.begin(modem.getDefaultBaudRate());  // set baud rate of the serial connection to match the modem
  while((!loraSerial) && (millis()) < 10000){}   // wait until the serial bus is available
  
  while(!device.initABP(DEV_ADDR, APPSKEY, NWKSKEY));
  debugSerial.println("Ready to send data"); 
  
}

void loop() {
 /* converts the value into distance */
  double Sensordistance1 = Sensordistance(SENSOR1_TRIGPIN,SENSOR1_ECHOPIN);
  delayMicroseconds (50);
  double Sensordistance3 = Sensordistance(SENSOR3_TRIGPIN,SENSOR3_ECHOPIN);
  delayMicroseconds (50);
  double Sensordistance2 = Sensordistance(SENSOR2_TRIGPIN,SENSOR2_ECHOPIN);
  delayMicroseconds (50);
  double Sensordistance4 = Sensordistance(SENSOR4_TRIGPIN,SENSOR4_ECHOPIN);
  delayMicroseconds (50);
  
  if (CurrentState == CLEAR )
  {
    if (Sensordistance1 < X && Sensordistance1 != 0 || Sensordistance3 < X && Sensordistance3 != 0)
    {
      // One person has  walked
      CurrentState = SENSOR_1_TRIGGERED;
      People = 1;
    }
    else if (Sensordistance2 < X && Sensordistance2 != 0 || Sensordistance4 < X && Sensordistance4 != 0 )
    {
      // One person has  walked
      CurrentState = SENSOR_2_TRIGGERED;
      People = 1;
    }
  }
  else if (CurrentState == SENSOR_1_TRIGGERED )
  {
    if (Sensordistance1 < Y && Sensordistance1 != 0 && Sensordistance3 < Y && Sensordistance3 != 0)
    {
      // two person has  walked
      People = 2;
    }
    if (Sensordistance2 < X && Sensordistance2 != 0 || Sensordistance4 < X && Sensordistance4 != 0 )
    {
      // One person has  walked
      CurrentState = BLOCKED;
      state = change;
      direction1Counter += People;
      delay(400);
    } 
  }
  else if (CurrentState == SENSOR_2_TRIGGERED)
  {
    if (Sensordistance2 < Y && Sensordistance2 != 0 && Sensordistance4 < Y && Sensordistance4 != 0 )
    {
      People = 2;
    }
    if (Sensordistance1 < X && Sensordistance1 != 0 || Sensordistance3 < X && Sensordistance3 != 0)
    {
      // One person has  walked
      CurrentState = BLOCKED;
      state = change_opp;
      direction2Counter += People;
      delay(400);
    }
  }
  else if (CurrentState == BLOCKED)
  {
    if (Sensordistance1 > X && Sensordistance2 > X && Sensordistance3 > X && Sensordistance4 > X)
    {
      CurrentState =  CLEAR;
      People = 0;
    }
  }
 
int total = direction1Counter - direction2Counter;
debugSerial.print ("Sensor 1 :");
debugSerial.print (Sensordistance1);
debugSerial.print ("||Sensor 3  :");
debugSerial.print (Sensordistance3);
debugSerial.print ("|| Sensor 2 :");
debugSerial.print (Sensordistance2);
debugSerial.print ("||Sensor 4  :");
debugSerial.print (Sensordistance4);
debugSerial.print ("||Counter 1 :");
debugSerial.print (direction1Counter);
debugSerial.print ("|| Counter 2 :");
debugSerial.print (direction2Counter);
debugSerial.print ("|| Total :");
debugSerial.println (total);
debugSerial.println ();

 if (total < 0)
 { total = 0; }
     
   if(state == change)
   {
   sendValue(total);
   state = 0;
   }
   if (state == change_opp)
   {
   sendValue(total);
   state = 0;
   } 
     
    if (Sensordistance1 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, LOW);
    digitalWrite(digit4, LOW);
    lighterror(0);
    lighterror(1);
    
    if (Sensordistance2 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, LOW);
    digitalWrite(digit4, LOW);
    lighterror(0);
    lighterror(5);
    if (Sensordistance3 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, LOW);
    lighterror(0);
    lighterror(11);
    
    if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(14);
    }
    }
    else if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, LOW);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(12);
    }
    }
    else if (Sensordistance3 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, LOW);
    lighterror(0);
    lighterror(6);
    if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(13);
    }
    }
    else if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, HIGH);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, LOW);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(7);
    }
    }
    else if (Sensordistance2 == 0)
    {
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, LOW);
    digitalWrite(digit4, LOW);
    lighterror(0);
    lighterror(2);
    
    if (Sensordistance3 == 0)
    {
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, LOW);
    lighterror(0);
    lighterror(8);
    
    if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(13);
    }
    }
    else if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, HIGH);
    digitalWrite(digit3, LOW);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(9);
    
    }
    }
    else if (Sensordistance3 == 0)
    {
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, LOW);
    lighterror(0);
    lighterror(3);
    
    if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, HIGH);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(10);
    }
    }
    else if (Sensordistance4 == 0)
    {
    digitalWrite(digit1, LOW);
    digitalWrite(digit2, LOW);
    digitalWrite(digit3, LOW);
    digitalWrite(digit4, HIGH);
    lighterror(0);
    lighterror(4);
    }
    else{
     displayNumber(total);}
  
}

void process()
{
  while(device.processQueue() > 0)
  {
    debugSerial.print("QueueCount: ");
    debugSerial.println(device.queueCount());
    delay(1000);
  }
}
void sendValue(int16_t counter)
{
  #ifdef CONTAINERS
  container.addToQueue(counter, INTEGER_SENSOR, false);
  process();
  #endif
  
  #ifdef CBOR
  payload.reset();
  payload.map(1);
  payload.addInteger(counter, "15");
  payload.addToQueue(false);
  process();
  #endif

  #ifdef BINARY  
  payload.reset();
  payload.addInteger(counter);
  payload.addToQueue(false);
  process();
  #endif
}

double Sensordistance(int trigPin,int echoPin)
{
long duration;
double distance;

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;


return distance;

}

void displayNumber(int toDisplay) {
#define DISPLAY_BRIGHTNESS  500

#define DIGIT_ON  HIGH
#define DIGIT_OFF  LOW

  long beginTime = millis();

  for(int digit = 4 ; digit > 0 ; digit--) {

    //Turn on a digit for a short amount of time
    switch(digit) {
    case 1:
      digitalWrite(digit1, DIGIT_ON);
      break;
    case 2:
      digitalWrite(digit2, DIGIT_ON);
      break;
    case 3:
      digitalWrite(digit3, DIGIT_ON);
      break;
    case 4:
      digitalWrite(digit4, DIGIT_ON);
      break;
    }

    //Turn on the right segments for this digit
    lightNumber(toDisplay % 10);
    toDisplay /= 10;

    delayMicroseconds(DISPLAY_BRIGHTNESS); 
    //Display digit for fraction of a second (1us to 5000us, 500 is pretty good)

    //Turn off all segments
    lightNumber(10); 

    //Turn off all digits
    digitalWrite(digit1, DIGIT_OFF);
    digitalWrite(digit2, DIGIT_OFF);
    digitalWrite(digit3, DIGIT_OFF);
    digitalWrite(digit4, DIGIT_OFF); 
  }

  while( (millis() - beginTime) < 10) ; 
  //Wait for 20ms to pass before we paint the display again
}

//Given a number, turns on those segments
//If number == 10, then turn off number
void lightNumber(int numberToDisplay) {

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

  switch (numberToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 1:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 4:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;

  case 8:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 9:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;

  case 10:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}
void lighterror(int errorToDisplay) {

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

  switch (errorToDisplay){

  case 0:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
    case 1:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
    case 2:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
    case 3:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
   case 4:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
    case 5:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
    case 6:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
    case 7:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
    case 8:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
    case 9:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
    case 10:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_OFF);
    digitalWrite(segG, SEGMENT_OFF);
    break;
    case 11:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_OFF);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
    case 12:
    digitalWrite(segA, SEGMENT_ON);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_OFF);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_ON);
    break;
    case 13:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_ON);
    digitalWrite(segC, SEGMENT_ON);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
    case 14:
    digitalWrite(segA, SEGMENT_OFF);
    digitalWrite(segB, SEGMENT_OFF);
    digitalWrite(segC, SEGMENT_OFF);
    digitalWrite(segD, SEGMENT_ON);
    digitalWrite(segE, SEGMENT_ON);
    digitalWrite(segF, SEGMENT_ON);
    digitalWrite(segG, SEGMENT_OFF);
    break;
  }
}
