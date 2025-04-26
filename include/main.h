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
#include <vector>
#include <ArduinoJson.h>

// Extern declarations for global variables
extern Adafruit_SSD1306 display;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;
extern bool pump1Status;
extern bool pump2Status;
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
extern bool automaticWateringStatus;
extern float sensor1Lower, sensor1Upper, sensor2Lower, sensor2Upper;

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
void checkScheduler();
void updateDisplay();
void checkWiFiStatus();
void handleStatus();
void handleSaveWiFiConfig();
void handleSaveAPConfig();
void automaticWatering();
void syncTime();
void pumpStatusUpdate();
void handleSchedulePage();
void handleDeleteSchedule();
void handleAddSchedule();
void handleResetData();
void handleGetThresholds();
void handleSaveThresholds();
void checkButtonPressed();
bool addConditionalSchedule(
    uint8_t pumpNumber,
    uint8_t hour, 
    uint8_t minute, 
    void (*callback)(bool, uint8_t), 
    String desc,
    float sensor1Limit, 
    float sensor2Limit,
    bool bothSensorsRequired,
    bool enabled
  );
  bool addTimedSchedule(
    uint8_t pumpNumber,
    uint8_t hour, 
    uint8_t minute,
    void (*callback)(bool, uint8_t), 
    String desc,
    unsigned long durationMinutes,
    bool enabled
  );
void pumpControl(bool state, uint8_t pumpNumber);

enum ScheduleType {
    CONDITIONAL,  // Active until sensor thresholds met
    TIMED         // Active for specific duration
  };
  
  // Schedule structure
  struct Schedule {
    uint8_t pumpNumber;
    ScheduleType type;
    uint8_t hour;
    uint8_t minute;
    void (*callback)(bool, uint8_t);  // Function to call with state (true=start, false=end)
    String description;
    bool enabled;
    
    // Conditional schedule parameters
    float sensor1Threshold;
    float sensor2Threshold;
    bool requireBothSensors;
    
    // Timed schedule parameters
    unsigned long durationSeconds;  // Duration for timed schedules
    
    // Runtime state
    bool isActive;
    unsigned long activeSince;
  };

  struct ScheduleStorage {
    uint8_t pumpNumber;
    uint8_t type;              // Save ScheduleType as uint8_t
    uint8_t hour;
    uint8_t minute;
    char description[50];      // Fixed-length char array instead of String
    bool enabled;
    float sensor1Threshold;
    float sensor2Threshold;
    bool requireBothSensors;
    unsigned long durationSeconds;
  };

ScheduleStorage fromSchedule(const Schedule& s);  
extern std::vector<Schedule> schedules;
std::vector<Schedule> loadStoredSchedules();
void saveSchedule(std::vector<Schedule> schedules);


#endif // MAIN_H