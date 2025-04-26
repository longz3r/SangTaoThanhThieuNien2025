#include "main.h"

void handleResetData() {
    server.send(200, "text/plain", "Đang xoá dữ liệu đã lưu...");
    displayMessage("Dang khoi dong lai", 1000);
    config.clear();
    // Delay to allow response to be sent
    delay(1000);
    ESP.restart();
}