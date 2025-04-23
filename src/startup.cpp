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

  if (!wifiConfigured) {
    AP_SSID = config.getString("AP_SSID");
    AP_PASS = config.getString("AP_PASS");
    

    WiFi.softAP(AP_SSID, AP_PASS);
  }

  server.on("/", handleRoot);
    server.on("/toggle", handleToggle);
    server.on("/sensorData", HTTP_GET, []() {
      String json = "{\"temperature\": " + String(rtc.getTemperature()) + 
            ", \"humidity\": {\"sensor1\": " + String(sensor1percent) + 
            ", \"sensor2\": " + String(sensor2percent) + "}}";
      server.send(200, "application/json", json);
    });
    
    server.on("/saveWiFiConfig", HTTP_POST, []() {
      String ssid = server.arg("ssid");
      String password = server.arg("password");
      
      if (ssid.length() == 0) {
        server.send(400, "text/plain", "SSID cannot be empty");
        return;
      }
      
      // Save to preferences
      config.putString("ssid", ssid);
      config.putString("pass", password);
      
      server.send(200, "text/plain", "Đã lưu thông tin WiFi");
      displayMessage("Dang khoi dong lai", 1000);
      
      // Delay to allow response to be sent
      delay(1000);
      ESP.restart();
    });
    server.begin();
}