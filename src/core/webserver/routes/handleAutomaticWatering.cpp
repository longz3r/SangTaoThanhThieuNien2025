#include "main.h"

void handleGetThresholds() {
    JsonDocument doc;

    doc["sensor1Lower"] = sensor1Lower;
    doc["sensor1Upper"] = sensor1Upper;
    doc["sensor2Lower"] = sensor2Lower;
    doc["sensor2Upper"] = sensor2Upper;
    doc["automaticWatering"] = automaticWateringStatus;

    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

void handleSaveThresholds() {
    if (server.hasArg("plain")) {
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, server.arg("plain"));
    
        if (!error) {
          config.putFloat("sensor1Lower", doc["sensor1Lower"]);
          config.putFloat("sensor1Upper", doc["sensor1Upper"]);
          config.putFloat("sensor2Lower", doc["sensor2Lower"]);
          config.putFloat("sensor2Upper", doc["sensor2Upper"]);
          config.putBool("AUTO_WATER", doc["automaticWatering"]);

          sensor1Lower = doc["sensor1Lower"];
          sensor1Upper = doc["sensor1Upper"];
          sensor2Lower = doc["sensor2Lower"];
          sensor2Upper = doc["sensor2Upper"];
          automaticWateringStatus = doc["automaticWatering"];

    
          server.send(200, "text/plain", "OK");
          return;
        }
      }
      server.send(400, "text/plain", "Bad Request");
}