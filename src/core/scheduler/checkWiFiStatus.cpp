#include "main.h"

void checkWiFiStatus() {
    if (WiFi.status() != WL_CONNECTED) {
        wifiConfigured = false;
    } else {
        wifiConfigured = true;
    }
}