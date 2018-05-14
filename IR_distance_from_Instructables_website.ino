
#define debugSerial SerialUSB

const int irsensorpin = 1;

void setup() {
// put your setup code here, to run once:
debugSerial.begin(57600); }

void loop(){
// Reads the InfraRed sensor analog values and convert into distance.
int sensorValue = analogRead(irsensorpin);
double IRdistance = 187754 * pow(sensorValue, -1.51);
double IRdistancescale = IRdistance * 0.625;

debugSerial.println(IRdistancescale);
// A delay is added for a stable and precise input
delay(150);
}
