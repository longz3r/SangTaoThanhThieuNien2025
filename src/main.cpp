// setup.cpp
#include "main.h"
#include <TaskScheduler.h>

// Global variable definitions
Adafruit_SSD1306 display(128, 64, &Wire, -1);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
bool pump1Status = false;
bool pump2Status = false;
bool wifiConfigured = false;
bool shouldStartAP = false;
Preferences config;
WebServer server(80);
RTC_DS3231 rtc;
float sensor1percent;
float sensor2percent;
DateTime now;
char daysOfTheWeek[7][12] = {"CN", "T2", "T3", "T4", "T5", "T6", "T7"};
float* humidity;
Scheduler runner;
bool automaticWateringStatus;
std::vector<Schedule> schedules;
float sensor1Lower, sensor1Upper, sensor2Lower, sensor2Upper;
int lastButtonCheck1, lastButtonCheck2 = 0;

Task tDisplay(100, TASK_FOREVER, &updateDisplay);
Task tPumpStatusUpdate(100, TASK_FOREVER, &pumpStatusUpdate);
Task tScheduler(5000, TASK_FOREVER, &checkScheduler); // Every 5s
Task tCheckWiFiStatus (5000, TASK_FOREVER, &checkWiFiStatus); // Every 5s
Task tAutomaticWatering (5000, TASK_FOREVER, &automaticWatering); // Every 5s
Task tCheckButtonPressed (100, TASK_FOREVER, &checkButtonPressed); // Every 5s

void setup() {
  config.begin("config", false);
  // Config 
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  timeClient.setTimeOffset(7*3600); // GMT+7

  automaticWateringStatus = config.getBool("AUTO_WATER");
  sensor1Lower = config.getFloat("sensor1Lower");
  sensor1Upper = config.getFloat("sensor1Upper");
  sensor2Lower = config.getFloat("sensor2Lower");
  sensor2Upper = config.getFloat("sensor2Upper");


  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(4, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(17, INPUT_PULLUP);
  pinMode(2, OUTPUT);

  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);

  // Initialize the scheduler
  runner.init();
  
  // Add tasks to scheduler
  runner.addTask(tDisplay);
  runner.addTask(tScheduler);
  runner.addTask(tCheckWiFiStatus);
  runner.addTask(tAutomaticWatering);
  runner.addTask(tPumpStatusUpdate);
  runner.addTask(tCheckButtonPressed);

  // Enable tasks
  tDisplay.enable();
  tScheduler.enable();
  tCheckWiFiStatus.enable();
  tAutomaticWatering.enableDelayed(5000);
  tPumpStatusUpdate.enable();
  tCheckButtonPressed.enableDelayed(3000);

  startup();
}

void loop() {
  runner.execute();
  server.handleClient();
}