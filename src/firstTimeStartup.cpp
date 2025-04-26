#include <main.h>

void firstTimeStartup() {
  if (!config.isKey("initialized")) {
    std::vector<Schedule> firstTimeSchedule;
    Schedule newSchedule = {
      1,
      CONDITIONAL,
      6,
      30,
      pumpControl,
      "Tưới nước buổi sáng",
      true,
      50,
      60,
      false,
      0,  // duration not used
      true,
      0
    };
    
    firstTimeSchedule.push_back(newSchedule);

    newSchedule = {
      3,
      TIMED,
      17,
      15,
      pumpControl,
      "Tưới nước buổi chiều",
      true,
      0,
      0,
      false,
      300,  // duration not used
      true,
      0
    };
    
    firstTimeSchedule.push_back(newSchedule);

    saveSchedule(firstTimeSchedule);
    config.putString("ssid", "VNPT VIET HA 4G");
    config.putString("pass", "08092008");

    config.putString("AP_SSID", "ESP32");
    config.putString("AP_PASS", "12345678");

    config.putBool("AUTOMATIC_WATERING", false);
    config.putBool("initialized", true);
  }
}