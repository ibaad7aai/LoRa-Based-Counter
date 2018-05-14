#define debugSerial SerialUSB
#define Measuring_Dis 150

const int IRpin = 2; 
const int SENSOR_TRIGPIN = 4;
const int SENSOR_ECHOPIN = 5; 
int incorrect = 0;
int inaccurate = 0;

double X = (Measuring_Dis * 0.85); // - 15% tollerance range
double Y = (Measuring_Dis * 1.15); // + 15% tollerance range

double IRsensor(int pin);
double UltrasonicSensor(int trigPin,int echoPin);


void setup() {
  // put your setup code here, to run once:
pinMode(SENSOR_TRIGPIN, OUTPUT); // Sets the trigPin as an Output
pinMode(SENSOR_ECHOPIN, INPUT); // Sets the echoPin as an Input

 debugSerial.begin(9600);    
}

void loop() {
  // put your main code here, to run repeatedly:
double IRdistance[250];
double Sensor[250];

for (int i = 1; i <= 100; i++)
{
  

IRdistance[i] = IRsensor(IRpin);
delay(10);
Sensor[i] = UltrasonicSensor(SENSOR_TRIGPIN,SENSOR_ECHOPIN);
debugSerial.print(i);
debugSerial.print(" : IR sensor : ");
debugSerial.print(IRdistance[i]);
debugSerial.print("  || ultrasonic sensor : ");
debugSerial.println(Sensor[i]);



if (IRdistance[i] < X || IRdistance[i] > Y)
{
 incorrect= incorrect+1;

}
 if (Sensor[i] < X || Sensor[i] > Y)
{

 inaccurate = inaccurate + 1;
}
if (i == 100)
{
debugSerial.print("IRerror :");
debugSerial.print(incorrect);
debugSerial.print(" USerror :"); 
debugSerial.println(inaccurate);
incorrect = 0;
inaccurate = 0;
}
}
delay(5000);

}
double IRsensor(int pin)
{
int SensorValue  = analogRead(pin);
float Volts = SensorValue * (3.3/1023); 
double distance =  12.5* pow(Volts, -1.539) ;

  return distance;
}

double UltrasonicSensor(int trigPin,int echoPin)
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

