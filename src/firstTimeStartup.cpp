#include <main.h>

void firstTimeStartup() {
  if (!config.isKey("initialized")) {
    config.putString("ssid", "ssid");
    config.putString("pass", "pass");

    config.putString("AP_SSID", "ESP32");
    config.putString("AP_PASS", "12345678");

    config.putBool("initialized", true);
  }
}