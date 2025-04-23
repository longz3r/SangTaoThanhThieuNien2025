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
#include "RTClib.h"

struct Sensor {
    float humidity1;
    float humidity2;    
};

// Extern declarations for global variables
extern Adafruit_SSD1306 display;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern bool pin1Status;
extern bool pin2Status;
extern bool wifiConfigured;
extern bool shouldStartAP;
extern Preferences config;
extern WebServer server;
extern String AP_SSID;
extern String AP_PASS;
extern RTC_DS3231 rtc;
extern float sensor2percent;
extern float sensor1percent;
extern String ssid;
extern String pass;
extern DateTime now;
extern char daysOfTheWeek[7][12];
extern float* humidity;

// Bitmap declarations
extern const unsigned char tick[];
extern const unsigned char cross[];
extern const unsigned char degree[];

// Function declarations
String generateHTML();
void handleRoot();
void handleToggle();
void displayStatusWithWiFi(float sensor1percent, float sensor2percent);
void displayStatusWithoutWiFi(float sensor1percent, float sensor2percent);
void displayMessage(String message, int time);
void setup();
void loop();
void firstTimeStartup();
void startup();
float readTemperature();
Sensor readHumidity();

#endif // MAIN_H