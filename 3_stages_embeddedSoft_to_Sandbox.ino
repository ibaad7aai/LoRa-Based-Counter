#include <Sodaq_RN2483.h>

#define debugSerial SerialUSB
#define loraSerial Serial2

#define X 19
#define Y 7

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


bool OTAA = false;

// ABP
// USE YOUR OWN KEYS!
const uint8_t devAddr[4] =
{
 0x5D,0xA8,0xE4,0xA8
};

// USE YOUR OWN KEYS!
const uint8_t appSKey[16] =
{
 0x7D,0x21,0x41,0x02,0x77,0xED,0x43,0x10,0xCA,0x09,0xC2,0xF9,0xCB,0x95,0x1C,0x34
};

// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{
 0x16,0x11,0x1A,0xA5,0xC4,0x60,0x64,0x3C,0xDD,0xD3,0x69,0x87,0x1A,0x79,0x5C,0xF3
};
// OTAA
const uint8_t DevEUI[8] =
{
 
};

const uint8_t AppEUI[16] =
{
  
};

const uint8_t AppKey[16] =
{
  

};

// LED 7  segment Display
int digit1 = 1; //PWM Display pin 1
int digit2 = 13; //PWM Display pin 2
int digit3 = 8; //PWM Display pin 8
int digit4 = 6; //PWM Display pin 0

int segA = 0; 
int segB = A0; 
int segC = A1; 
int segD = A2; 
int segE = A3; 
int segF = A4; 
int segG = A5; 

void setup()
{
  while ((!debugSerial) && (millis() < 10000)){
    // Wait 10 seconds for debugSerial to open
  }
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

pinMode(SENSOR1_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR1_ECHOPIN, INPUT); // Sets the echoPin as an Input
pinMode(SENSOR2_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR2_ECHOPIN, INPUT); // Sets the echoPin as an Input
pinMode(SENSOR3_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR3_ECHOPIN, INPUT); // Sets the echoPin as an Input
pinMode(SENSOR4_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR4_ECHOPIN, INPUT); // Sets the echoPin as an Input

  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  setupLoRaABP();
}

void setupLoRa(){
  if(!OTAA){
    // ABP
    setupLoRaABP();
  } else {
    //OTAA
    setupLoRaOTAA();
  }
}
void setupLoRaABP(){  
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false))
  {
    debugSerial.println("Communication to LoRaBEE successful.");
  }
  else
  {
    debugSerial.println("Communication to LoRaBEE failed!");
  }
}

void setupLoRaOTAA()
{
  debugSerial.println(LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false));
  
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false))
  {
    debugSerial.println("Network connection successful.");
  }
  else
  {
    debugSerial.println("Network connection failed!");
  }
}

void loop()
{
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
    {  // One person has  walked
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
    { // two person has  walked
      People = 2;
      }
    if (Sensordistance2 < X && Sensordistance2 != 0 || Sensordistance4 < X && Sensordistance4 != 0 )
    {  // One person has  walked
      CurrentState = BLOCKED;
      state = change;
      direction1Counter += People;
      delay(400);} 
      }
  else if (CurrentState == SENSOR_2_TRIGGERED)
  {
    if (Sensordistance2 < Y && Sensordistance2 != 0 && Sensordistance4 < Y && Sensordistance4 != 0 )
    { 
      People = 2;
      }
    if (Sensordistance1 < X && Sensordistance1 != 0 || Sensordistance3 < X && Sensordistance3 != 0)
    {// One person has  walked
      CurrentState = BLOCKED;
      state = change_opp;
      direction2Counter += People;
      delay(400);}
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

String data = String(total);  
   
if (total < 0)
{ total = 0; }
   
   if(state == change ||state == change_opp )
   {
   Transmission(data);
   delay(10); 
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


void Transmission(String Payload)
{
  String reading = Payload;
  debugSerial.println(reading);
  
    switch (LoRaBee.send(1, (uint8_t*)reading.c_str(), reading.length()))
    {
    case NoError:
      debugSerial.println("Successful transmission.");
      break;
    case NoResponse:
      debugSerial.println("There was no response from the device.");
      break;
    case Timeout:
      debugSerial.println("Connection timed-out. Check your serial connection to the device! Sleeping for 20sec.");
      delay(20000);
      break;
    case PayloadSizeError:
      debugSerial.println("The size of the payload is greater than allowed. Transmission failed!");
      break;
    case InternalError:
      debugSerial.println("Oh No! This shouldn't happen. Something is really wrong! The program will reset the RN module.");
      setupLoRa();
      break;
    case Busy:
      debugSerial.println("The device is busy. Sleeping for 10 extra seconds.");
      delay(10000);
      break;
    case NetworkFatalError:
      debugSerial.println("There is a non-recoverable error with the network connection. The program will reset the RN module.");
      setupLoRa();
      break;
    case NotConnected:
      debugSerial.println("The device is not connected to the network. The program will reset the RN module.");
      setupLoRa();
      break;
    case NoAcknowledgment:
      debugSerial.println("There was no acknowledgment sent back!");
      break;
    default:
      break;
    }
    // Delay between readings
    // 60 000 = 1 minute
    delay(1000); // 5 sec



   
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
