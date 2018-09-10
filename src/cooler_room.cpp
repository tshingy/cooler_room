#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>

#include "index.h"

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress AC = { 0x28, 0xFD, 0x02, 0x3B, 0x1A, 0x13, 0x01, 0xBE };    // DS18B20 device addresses
DeviceAddress Room = { 0x28, 0x71, 0x7D, 0x2F, 0x1A, 0x13, 0x01, 0x1D };

int heaterPin = 0;    // pin for heater on NodeMCU
int heaterOff = 1023;    // pin signal
int heaterOn = 0;  // pin signal

// Hard coded temperatures. Values can be change with web interface
float roomMin = 45.00;
float acMin = 35.00;

bool firstRun = true;

float room, ac;
String heaterStatus = "OFF";

// Time interval values
long previousMillis = 0;
long interval = 600000;

// HTML to auto update temperature sensors value
const String s0 = "<div style=\"margin-bottom: 20px; text-align: center; font-size: 2.5em; text-decoration: underline;\">HEATER ";
const String s1 = "</div><div class=\"divTable\" style=\"width: auto; margin: auto;\"><div class=\"divTableBody\"><div class=\"divTableRow\"><div class=\"divTableCell\" style=\"text-align: right;\">Room Temp:</div><div class=\"divTableCell\" style=\"text-align: left;\">";
const String s2 = "</div></div><div class=\"divTableRow\"><div class=\"divTableCell\" style=\"text-align: right;\">AC Temp:</div><div class=\"divTableCell\">";
const String s3 = "</div></div><div class=\"divTableRow\"><div class=\"divTableCell\" style=\"text-align: right;\">&nbsp;</div><div class=\"divTableCell\">&nbsp;</div></div><div class=\"divTableRow\"><div class=\"divTableCell\" style=\"text-align: right;\">Room Setting:</div><div class=\"divTableCell\">";
const String s4 = "</div></div><div class=\"divTableRow\"><div class=\"divTableCell\" style=\"text-align: right;\">AC Setting:</div><div class=\"divTableCell\">";
const String s5 = "</div></div></div>";
String s = MAIN_page;

ESP8266WebServer server(80); // Initialize webserver on Port 80

void handleRoot() {
  server.send(200, "text/html", s);
}

void setValue() {

  if (server.arg("set_room") != ""){
    double h = server.arg("set_room").toFloat();
    roomMin = h;
    previousMillis -= interval;
    }
  if (server.arg("set_ac") != "") {
    double a = server.arg("set_ac").toFloat();
    acMin = a;
    previousMillis -= interval;
  }
  server.send(200, "text/html", s);
}

void updating() {
  String text = s0 + heaterStatus;
  text += s1 + String(room);
  text += s2 + String(ac);
  text += s3 + String(roomMin);
  text += s4 + String(acMin);
  text += s5;
  
  server.send(200, "text/html", text);
}

void setup() {
  
  Serial.begin(115200);
  Serial.println("Cooler Room Start\n");

  /* Configure NodeMCU WiFi if it's not already configured. 
   * To setup NodeMCU WiFi, use router to find NodeMCU
   * IP Address and access it from Computer or Smartphone.
  */
  WiFiManager wifiManager;
  WiFi.hostname("CoolerRoom");
  wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  wifiManager.autoConnect("CoolerRoom Setup");
 
  
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  

  // handling web requests
  server.on("/updating", updating);
  server.on("/set_value", setValue);
  server.on ( "/", handleRoot);
    
  server.begin();
  Serial.println ( "HTTP server started" );

  pinMode(heaterPin, OUTPUT);
  sensors.begin();
  sensors.setResolution(Room, 10);
  sensors.setResolution(AC, 10);
}

void loop() {
  server.handleClient();

  sensors.requestTemperatures();  // retrieve temperature
  room = getTemperature(Room);
  ac = getTemperature(AC);

  if (firstRun){
    if (room < roomMin || ac < acMin) {   // turn heater off if room or ac temp is reached
      heaterStatus = "OFF";
      analogWrite(heaterPin, heaterOff);
    } else {    // turn on heater if room or ac temp not reached
      heaterStatus = "ON";
      analogWrite(heaterPin, heaterOn);
    }
    firstRun = false;
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {    // how often to check temperature fluctuations
    previousMillis = currentMillis;

    if (room < roomMin || ac < acMin) {   // turn heater off if room or ac temp is reached
      heaterStatus = "OFF";
      analogWrite(heaterPin, heaterOff);
    } else {    // turn on heater if room or ac temp not reached
      heaterStatus = "ON";
      analogWrite(heaterPin, heaterOn);
    }
  } else if (ac < acMin) {
    analogWrite(heaterPin, heaterOff);
  }
}

float getTemperature(DeviceAddress deviceAddress) {  // function to retrieve temperature from DS18B20 sensors
  float tempC = sensors.getTempC(deviceAddress);
  if (tempC != NULL){
    return ((tempC * 9)/5) + 32;
  } else {
    Serial.println("Temp Module Error");
    return -100.00;
  }
}
