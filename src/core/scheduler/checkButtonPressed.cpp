#include <main.h>

void checkButtonPressed() {
    if (!digitalRead(4)) {
        digitalWrite(2, HIGH);
        displayMessage("Dang reset du leu", 1000);
        config.clear();
        // Delay to allow response to be sent
        delay(1000);
        ESP.restart();
      } else {
        digitalWrite(2, LOW);
      }
    if (!digitalRead(16)) {
      if (millis() - lastButtonCheck1 < 200) return;
      pump1Status = !pump1Status;
      lastButtonCheck1 = millis();
    }
    if (!digitalRead(17)) {
      if (millis() - lastButtonCheck2 < 200) return;
      pump2Status = !pump2Status;
      lastButtonCheck2 = millis();
    }
}