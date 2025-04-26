#include "main.h"

void handleSaveAPConfig() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    
    if (ssid.length() == 0 || password.length() < 8) {
        server.send(400, "text/plain", "Tên Access Point không thể để trống");
        return;
    }

    if (password.length() < 8) {
        server.send(400, "text/plain", "Mật khẩu cần tối thiểu 8 kí tự");
        return;
    }
      
    // Save to preferences
    config.putString("AP_SSID", ssid);
    config.putString("AP_PASS", password);
      
    server.send(200, "text/plain", "Đã lưu thông tin Access Point");
    displayMessage("Dang khoi dong lai", 1000);
      
    // Delay to allow response to be sent
    delay(1000);
    ESP.restart();
}