#include "main.h"

void handleSensorData() {
    String json = "{\"temperature\": " + String(rtc.getTemperature()) + 
    ", \"humidity\": {\"sensor1\": " + String(sensor1percent) + 
    ", \"sensor2\": " + String(sensor2percent) + "}}";
    server.send(200, "application/json", json);
}