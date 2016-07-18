#include <LiquidCrystal.h>
#include <Nestor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FS.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266mDNS.h>
#include <core/ESP8266PinMap.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <utils/NestorOLED.h>
#include <utils/NestorTime.h>
Nestor *nestor;

void setup() {
 
 Serial.setDebugOutput(true);
 Serial.begin(9600);
 
 Serial.println("Starting...");

 Nestor::warmup();

 nestor = Nestor::getInstance();
  
 Serial.println("Nestor Instance created.");

 if (!nestor->initialize()) {
  Serial.println("Failed to initialize Nestor, rebooting...");
  nestor->reboot();
 }
}

void loop() {  
 nestor->loop();
}
