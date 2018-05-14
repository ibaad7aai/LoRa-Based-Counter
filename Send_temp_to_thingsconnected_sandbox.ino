#include <Sodaq_RN2483.h>
#define debugSerial SerialUSB
#define loraSerial Serial2
bool OTAA = false; 

// USE YOUR OWN KEYS!
const uint8_t devAddr[4] =
{ 0x1C,0xB6,0x7D,0x05 };
const uint8_t appSKey[16] =
{0x37,0xEC,0xBE,0x88,0x93,0xD1,0x3B,0x99,0x62,0xF1,0x6A,0x4D,0x70,0x24,0x1C,0x18};
// USE YOUR OWN KEYS!
const uint8_t nwkSKey[16] =
{ 0x9B,0xE0,0x37,0x39,0x8B,0x29,0x72,0x17,0xFE,0xEA,0xFE,0x23,0x7C,0xD2,0xCE,0x2A};
const uint8_t DevEUI[8] =
{ 0x58,0x32,0x2C,0x8B,0xF3,0x02,0xD5,0xE0};
const uint8_t AppEUI[16] = { };
const uint8_t AppKey[16] ={ };

void setup()
{  while ((!debugSerial) && (millis() < 10000))
{ // Wait 10 seconds for debugSerial to open 
  
}
  debugSerial.begin(57600);
  loraSerial.begin(LoRaBee.getDefaultBaudRate());
  setupLoRa(); 
}
void setupLoRa(){
  if(!OTAA){
    // ABP
    setupLoRaABP();}
 else {
    //OTAA
    setupLoRaOTAA();} }

void setupLoRaABP(){  
  if (LoRaBee.initABP(loraSerial, devAddr, appSKey, nwkSKey, false))
  {  debugSerial.println("Communication to LoRaBEE successful.") ; }
 
 else{ debugSerial.println("Communication to LoRaBEE failed!");}
}
void setupLoRaOTAA()
{ debugSerial.println(LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false));
  if (LoRaBee.initOTA(loraSerial, DevEUI, AppEUI, AppKey, false))
  { debugSerial.println("Network connection successful.");}
  else
  { debugSerial.println("Network connection failed!");} }
void loop()
{  String reading = getTemperature();
       switch (LoRaBee.send(1, (uint8_t*)reading.c_str(), reading.length()))
    { case NoError:
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
    case NetworkFatalError:debugSerial.println("There is a non-recoverable error with the network connection. The program will reset the RN module.");
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
      break; }
    delay(10000); }

String getTemperature()
{  //10mV per C, 0C is 500mV
  float mVolts = (float)analogRead(TEMP_SENSOR) * 3300.0 / 1023.0;
  float temp = (mVolts - 500.0) / 10.0;
  //debugSerial.println(temp);
  return String(temp);}

