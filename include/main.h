// main.h
#ifndef MAIN_H
#define MAIN_H

#include "Arduino.h"
#include "Wire.h"
#include "SPI.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Preferences.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Extern declarations for global variables
extern Adafruit_SSD1306 display;
extern const char* ssid;
extern const char* pass;
extern const char* apSSID;
extern const char* apPASS;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern bool pin1Status;
extern bool pin2Status;
extern bool wifiConfigured;
extern bool shouldStartAP;
extern Preferences preferences;
extern WebServer server;

// Bitmap declarations
extern const unsigned char tick[];
extern const unsigned char cross[];

// Function declarations
void startAPMode();
String generateHTML();
void handleRoot();
void handleToggle();
void displayStatus(float sensor1percent, float sensor2percent);
void setup();
void loop();

#endif