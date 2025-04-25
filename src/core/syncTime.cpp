#include <main.h>

void syncTime() {
    if (WiFi.status() == WL_CONNECTED && rtc.lostPower()) {
        timeClient.update();
        displayMessage("Dang dong bo thoi gian", 500);
        if (timeClient.isTimeSet()) {
          unsigned long epochTime = timeClient.getEpochTime();
          rtc.adjust(DateTime(epochTime));
          delay(1000);
        }
      }
}