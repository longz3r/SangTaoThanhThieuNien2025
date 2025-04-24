#include <main.h>

void handleToggle() {
    if (server.hasArg("pin")) {
      int pin = server.arg("pin").toInt();
    
      // Toggle the appropriate pin
      if (pin == 1) {
          pin1Status = !pin1Status;
          digitalWrite(12, pin1Status ? LOW : HIGH);
      } else if (pin == 2) {
          pin2Status = !pin2Status;
          digitalWrite(13, pin2Status ? LOW : HIGH);
      }
      
      // Return JSON response with current states
      String json = "{";
      json += "\"pin1Status\":" + String(pin1Status ? "true" : "false") + ",";
      json += "\"pin2Status\":" + String(pin2Status ? "true" : "false");
      json += "}";
      
      server.send(200, "application/json", json);
      return;
    }
    server.send(200, "application/json", "{}");
  }