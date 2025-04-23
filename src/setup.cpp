// setup.cpp
#include "main.h"

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

void setup() {
  config.begin("config", false);
  // Config 
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  WiFi.mode(WIFI_AP_STA);

  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);

  startup();
}