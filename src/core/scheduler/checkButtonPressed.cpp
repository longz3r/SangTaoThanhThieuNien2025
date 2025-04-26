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
}