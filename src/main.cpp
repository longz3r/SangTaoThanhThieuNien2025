// setup.cpp
#include "main.h"
#include <TaskScheduler.h>

// Global variable definitions
Adafruit_SSD1306 display(128, 64, &Wire, -1);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
bool pin1Status = false;
bool pin2Status = false;
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

Task tDisplay(100, TASK_FOREVER, &updateDisplay);   // Every 1s
Task tScheduler(5000, TASK_FOREVER, &checkScheduler); // Every 5s
Task tCheckWiFiStatus (5000, TASK_FOREVER, &checkWiFiStatus); // Every 5s

void setup() {
  config.begin("config", false);
  // Config 
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);

  // Initialize the scheduler
  runner.init();
  
  // Add tasks to scheduler
  runner.addTask(tDisplay);
  runner.addTask(tScheduler);
  runner.addTask(tCheckWiFiStatus);

  // Enable tasks
  tDisplay.enable();
  tScheduler.enable();
  tCheckWiFiStatus.enable();

  startup();
}

void loop() {
  runner.execute();

  // if (sensor1percent / 10 < 40) {
  //   digitalWrite(12, LOW);
  //   pin1Status = true;
  // } else {
  //   digitalWrite(12, HIGH);
  //   pin1Status = false;
  // }

  server.handleClient();
}