#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <TimeLib.h>
#include <String.h>

//const char* Ver = "4.1";          //Build Version
//const char* VerDate = "19/07/2020";          //Duild Date

#define OWPin D4
OneWire oneWire(OWPin);
DallasTemperature temperatureSensors(&oneWire);
int numberOfDevices;
DeviceAddress tempDeviceAddress; 

const char* Hssid     = "BlueEther_Tech_2G";       // ERASE prior to publishing
const char* Hpassword = "iwillnotgiveyoumykey";    //   "" ditto


#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "blueether"
#define AIO_KEY         "          "

#define AIO_SERVER_H      "10.1.1.56"
#define AIO_SERVERPORT_H  1883

unsigned int localPort = 2390;      // local port to listen for UDP packets

IPAddress timeServerIP; // pool.ntp.org NTP server address
const char* ntpServerName = "pool.ntp.org";
const int timeZone = 0;     // UTC
String strTimeStamp;
String strMQTT;


// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
WiFiClient clientH;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Client mqttH(&clientH, AIO_SERVER_H, AIO_SERVERPORT_H);

Adafruit_MQTT_Publish MQTT_Lounge     = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/lounge");
Adafruit_MQTT_Publish MQTT_Deck       = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/deck");
Adafruit_MQTT_Publish MQTT_BaseStation = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/base");
Adafruit_MQTT_Publish MQTT_GroundAir  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/groundair");
Adafruit_MQTT_Publish MQTT_Ground30   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ground30");
Adafruit_MQTT_Publish MQTT_Bed_Cady   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/bed_cady");
Adafruit_MQTT_Publish MQTT_Bed_Ismay  = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/bed_ismay");
Adafruit_MQTT_Publish MQTT_Bed_Master = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/bed_master");
Adafruit_MQTT_Publish MQTT_Ceiling    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ceiling");
Adafruit_MQTT_Publish MQTT_Basement   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/basement");
Adafruit_MQTT_Publish MQTT_Kitchen    = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/kitchen");

Adafruit_MQTT_Publish MQTT_LoungeH    = Adafruit_MQTT_Publish(&mqttH, "feeds/lounge");
Adafruit_MQTT_Publish MQTT_DeckH      = Adafruit_MQTT_Publish(&mqttH, "feeds/deck");
Adafruit_MQTT_Publish MQTT_BaseStationH=Adafruit_MQTT_Publish(&mqttH, "feeds/base");
Adafruit_MQTT_Publish MQTT_GroundAirH = Adafruit_MQTT_Publish(&mqttH, "feeds/groundair");
Adafruit_MQTT_Publish MQTT_Ground30H  = Adafruit_MQTT_Publish(&mqttH, "feeds/ground30");
Adafruit_MQTT_Publish MQTT_Bed_CadyH  = Adafruit_MQTT_Publish(&mqttH, "feeds/bed_cady");
Adafruit_MQTT_Publish MQTT_Bed_IsmayH = Adafruit_MQTT_Publish(&mqttH, "feeds/bed_ismay");
Adafruit_MQTT_Publish MQTT_Bed_MasterH= Adafruit_MQTT_Publish(&mqttH, "feeds/bed_master");
Adafruit_MQTT_Publish MQTT_CeilingH   = Adafruit_MQTT_Publish(&mqttH, "feeds/ceiling");
Adafruit_MQTT_Publish MQTT_BasementH  = Adafruit_MQTT_Publish(&mqttH, "feeds/basement");
Adafruit_MQTT_Publish MQTT_KitchenH   = Adafruit_MQTT_Publish(&mqttH, "feeds/kitchen");
Adafruit_MQTT_Publish MQTT_ALL        = Adafruit_MQTT_Publish(&mqttH, "feeds/ALL");


DeviceAddress addr_basestation= {0x28, 0xFF, 0x34, 0xDD, 0x80, 0x16, 0x03, 0x89};   //0
DeviceAddress addr_ceiling    = {0x28, 0xFF, 0xCE, 0x9E, 0x74, 0x16, 0x04, 0x6A};   //1
DeviceAddress addr_lounge     = {0x28, 0xFF, 0x7E, 0x30, 0x86, 0x16, 0x05, 0x05};   //2
DeviceAddress addr_bed_master = {0x28, 0xFF, 0xAD, 0xDE, 0x85, 0x16, 0x04, 0xF3};   //3
DeviceAddress addr_ground_air = {0x28, 0xFF, 0x9D, 0xDF, 0x85, 0x16, 0x04, 0xF8};   //4
DeviceAddress addr_ground_30  = {0x28, 0xFF, 0xBD, 0x97, 0x85, 0x16, 0x04, 0xFC};   //5
DeviceAddress addr_bed_cady   = {0x28, 0xFF, 0x63, 0xDC, 0x85, 0x16, 0x04, 0x74};   //6 red
DeviceAddress addr_deck       = {0x28, 0xFF, 0x8B, 0x73, 0x86, 0x16, 0x05, 0x19};   //7
DeviceAddress addr_bed_ismay  = {0x28, 0xFF, 0xB7, 0x4D, 0x85, 0x16, 0x03, 0xD8};   //8
//DeviceAddress addr_basement   = {0x28, 0xFF, 0xAD, 0xDE, 0x85, 0x16, 0x04, 0xF3};
//DeviceAddress addr_kitchen    = {0x28, 0xFF, 0x63, 0xDC, 0x85, 0x16, 0x04, 0x74};
typedef struct {
  const char* name;
  float degC;
  const uint8_t* DevID;
  Adafruit_MQTT_Publish* MQTT_Home;
  Adafruit_MQTT_Publish* MQTT_Ada;
} SensorSet;
const int no_sensors = 9;
 SensorSet TemperatureObj[no_sensors];

void printAddress(DeviceAddress &deviceAddress);
void listDevices(void);
void MQTT_connect();
void MQTT_connectH();
void doTemps(void);
void printHeaders(void);

time_t getNtpTime();
void digitalClockDisplay();
String printDigits(int digits);
void sendNTPpacket(IPAddress &address);

void setup(){
  Serial.begin(115200);
   delay (5000);
  Serial.println(F("Home Temp Monitor."));
  //CSV Format:
  //Time	 Deck	 Lounge	 BaseStation	 Ground_air	 Ground_30cm	Bed_Cady	Bed_Ismay	Bed_Master	Ceiling Basement
  TemperatureObj[0] = (SensorSet){"Deck",   0.0, addr_deck,        &MQTT_DeckH,      &MQTT_Deck}; 
  TemperatureObj[1] = (SensorSet){"Lounge", 0.0, addr_lounge,      &MQTT_LoungeH,    &MQTT_Lounge};
  TemperatureObj[2] = (SensorSet){"Base",   0.0, addr_basestation, &MQTT_BaseStationH,&MQTT_BaseStation};
  TemperatureObj[3] = (SensorSet){"Air",    0.0, addr_ground_air,  &MQTT_GroundAirH, &MQTT_GroundAir};
  TemperatureObj[4] = (SensorSet){"Soil",   0.0, addr_ground_30,   &MQTT_Ground30H,  &MQTT_Ground30};
  TemperatureObj[5] = (SensorSet){"Cady",   0.0, addr_bed_cady,    &MQTT_Bed_CadyH,  &MQTT_Bed_Cady};
  TemperatureObj[6] = (SensorSet){"Ismay",  0.0, addr_bed_ismay,   &MQTT_Bed_IsmayH, &MQTT_Bed_Ismay};
  TemperatureObj[7] = (SensorSet){"Master", 0.0, addr_bed_master,  &MQTT_Bed_MasterH,&MQTT_Bed_Master};
  TemperatureObj[8] = (SensorSet){"Ceiling",0.0, addr_ceiling,     &MQTT_CeilingH,   &MQTT_Ceiling};

  WiFi.mode(WIFI_STA);
  WiFi.begin(Hssid, Hpassword);

  int i = 0;
  Serial.print("Connecting to ");
  Serial.println(Hssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(i);
    //if(i>10) break;
    i++;
  }
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());
  
  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  // Start Dallas up the library
  
  temperatureSensors.begin();
  
  listDevices();
  //MQTT_connect();
  MQTT_connectH();
  printHeaders();
}

time_t prevDisplay = 0; // when the digital clock was displayed
int loopcount = 0;
void loop(){  
  if (timeStatus() != timeNotSet) {
    if (now() >= prevDisplay+30) { //update the display only if time has changed
      prevDisplay = now();
      digitalClockDisplay();
      if(loopcount<10) loopcount++;
      else {
        printHeaders();
        loopcount = 0;
      }
      //MQTT_connect();
      MQTT_connectH();
      //listDevices();
      doTemps();
    }
  }
}

// Print headers on serial
void printHeaders(void){
  Serial.println();
  for (int i=0; i < no_sensors; i++) {
    Serial.print(TemperatureObj[i].name);
    Serial.print("\t");
  }
  Serial.println();
}

// function to print a device address
void printAddress(DeviceAddress &deviceAddress) {
   // Serial.print("{");
  for (uint8_t i = 0; i < 8; i++){
      Serial.print("0x");
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      Serial.print(" ");
    
  }
   // Serial.print("}");
      
}


void doTemps(void){
  temperatureSensors.requestTemperatures(); // Send the command to get temperatures
  strMQTT = strTimeStamp;
  for (int i=0; i < no_sensors; i++) {
    TemperatureObj[i].degC = temperatureSensors.getTempC(TemperatureObj[i].DevID);
    if (TemperatureObj[i].degC == -127.00) TemperatureObj[i].degC  = 0;
    Serial.print(TemperatureObj[i].degC);
    Serial.print("\t");
    strMQTT += ",";
    strMQTT += TemperatureObj[i].degC;
    //TemperatureObj[i].MQTT_Ada->publish(TemperatureObj[i].degC);
    TemperatureObj[i].MQTT_Home->publish(TemperatureObj[i].degC);
  }

  Serial.println();
  //Serial.println(strMQTT);
   const char* buf = strMQTT.c_str();
  MQTT_ALL.publish(buf);
}

void listDevices(void){

    // Grab a count of devices on the wire
  Serial.println(F("DB: read no. of sensors..."));
  numberOfDevices = temperatureSensors.getDeviceCount();
  // locate devices on the bus
  Serial.print("\nLocating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++){
    // Search the wire for address
    if(temperatureSensors.getAddress(tempDeviceAddress, i)){
      
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println(" ... Setup Dallas Temp Sensors ...\n");
      temperatureSensors.setResolution(tempDeviceAddress, 12); //set ds18b20 to 12 data bits (0.06 deg c)
      
      //Serial.println();
    } else {
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}


void MQTT_connectH() {
  int8_t ret;

  // Stop if already connected.
  if (mqttH.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT at home... ");

  uint8_t retries = 3;
  while ((ret = mqttH.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqttH.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqttH.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         Serial.println("Reset..");
         ESP.restart();
       }
  }
  Serial.println("MQTT Home Connected!");
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT at Adafruit... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqttH.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         Serial.println("Reset..");
         //ESP.restart();
       }
  }
  Serial.println("MQTT Adafruit Connected!");
}


void digitalClockDisplay()
{
  // digital clock display of the time
  strTimeStamp = year();
  strTimeStamp += "-";
  strTimeStamp += printDigits(month());
  strTimeStamp += "-";
  strTimeStamp += printDigits(day());
  strTimeStamp += "T";
  strTimeStamp += printDigits(hour());
  strTimeStamp += ":";
  strTimeStamp +=printDigits(minute());
  strTimeStamp += ":";
  strTimeStamp +=printDigits(second());
  strTimeStamp += "Z";
  //Serial.println(strTimeStamp);
}

String printDigits(int digits)
{
  String s = "";
  if (digits < 10)
    s = '0';
  s += digits;
  return s;
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
