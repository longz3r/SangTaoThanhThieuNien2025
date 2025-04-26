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
    config.putString("ssid", "CHUA CAI DAT");
    config.putString("pass", "CHUA CAI DAT");

    config.putString("AP_SSID", "PlantWatering");
    config.putString("AP_PASS", "12345678");

    config.putBool("AUTO_WATER", false);
    config.putFloat("sensor1Lower", 30);
    config.putFloat("sensor1Upper", 50);
    config.putFloat("sensor2Lower", 40);
    config.putFloat("sensor2Upper", 70);

    config.putBool("initialized", true);
  }
}