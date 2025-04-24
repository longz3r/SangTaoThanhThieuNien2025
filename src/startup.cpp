#include <main.h>

String AP_SSID;
String AP_PASS;
String ssid;
String pass;

void startup() {
  // Adafruit splash screen
  display.display();
  firstTimeStartup();
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
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 5000) {
      delay(100);
      display.print(".");
      display.display();
    }

    if (WiFi.status() != WL_CONNECTED) {
      displayMessage("Ko the ket noi WiFi", 1500);
    } else {
      wifiConfigured = true;
    }
  }

  // if (!wifiConfigured) {
    AP_SSID = config.getString("AP_SSID");
    AP_PASS = config.getString("AP_PASS");
    
    WiFi.softAP(AP_SSID, AP_PASS);
  // }

  server.on("/", handleRoot);
    server.on("/toggle", handleToggle);
    server.on("/sensorData", HTTP_GET, handleSensorData);
    
    server.on("/saveWiFiConfig", HTTP_POST, []() {
      
    });
    server.begin();
}