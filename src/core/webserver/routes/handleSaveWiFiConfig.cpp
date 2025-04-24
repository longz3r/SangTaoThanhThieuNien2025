#include "main.h"

void handleSaveWiFiConfig() {
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
}