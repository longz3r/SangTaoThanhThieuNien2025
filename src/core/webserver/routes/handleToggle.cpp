#include <main.h>

void handleToggle() {
    if (server.hasArg("pin")) {
      int pin = server.arg("pin").toInt();
    
      // Toggle the appropriate pin
      if (pin == 1) {
          pump1Status = !pump1Status;
      } else if (pin == 2) {
          pump2Status = !pump2Status;
      }
      
      // Return JSON response with current states
      String json = "{";
      json += "\"pump1Status\":" + String(pump1Status ? "true" : "false") + ",";
      json += "\"pump2Status\":" + String(pump2Status ? "true" : "false");
      json += "}";
      
      server.send(200, "application/json", json);
      return;
    }
    server.send(200, "application/json", "{}");
  }