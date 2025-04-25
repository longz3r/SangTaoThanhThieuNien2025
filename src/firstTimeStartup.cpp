#include <main.h>

void firstTimeStartup() {
  if (!config.isKey("initialized")) {
    config.putString("ssid", "CHUA CAI DAT");
    config.putString("pass", "CHUA CAI DAT");

    config.putString("AP_SSID", "ESP32");
    config.putString("AP_PASS", "12345678");

    config.putBool("AUTOMATIC_WATERING", false);
    config.putBool("initialized", true);
  }
}