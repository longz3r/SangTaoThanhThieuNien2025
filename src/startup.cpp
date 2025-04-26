#include <main.h>

String AP_SSID;
String AP_PASS;
String ssid;
String pass;

void startup() {
  // Adafruit splash screen
  display.display();
  firstTimeStartup();
  schedules = loadStoredSchedules();
  delay(200);

  if (!rtc.begin()) {
    displayMessage("Khong tim thay RTC!", 2000);
  }

  ssid = config.getString("ssid");
  pass = config.getString("pass");

  if (config.isKey("ssid")) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Dang ket noi WiFi");
    display.display();
    WiFi.begin(config.getString("ssid"), config.getString("pass"));

    int startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
      delay(200);
      display.print(".");
      display.display();
    }

    if (WiFi.status() != WL_CONNECTED) {
      displayMessage("Ko the ket noi WiFi", 1500);
    } else {
      syncTime();
      wifiConfigured = true;
    }
  }

  AP_SSID = config.getString("AP_SSID");
  AP_PASS = config.getString("AP_PASS");
    
  WiFi.softAP(AP_SSID, AP_PASS);

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/saveWiFiConfig", HTTP_POST, handleSaveWiFiConfig);
  server.on("/saveAPConfig", HTTP_POST, handleSaveAPConfig);
  server.on("/lichtrinh", HTTP_GET, handleSchedulePage);
  server.on("/deleteSchedule", HTTP_GET, handleDeleteSchedule);
  server.on("/addSchedule", HTTP_POST, handleAddSchedule);
  server.on("/resetData", HTTP_GET, handleResetData);
  server.begin();
}