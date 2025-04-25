#include "main.h"

void handleStatus() {
    String json = "{\"temperature\": " + String(rtc.getTemperature()) + 
    ", \"pump1Status\":" + String(pump1Status ? "true" : "false") +
    ", \"pump2Status\":" + String(pump2Status ? "true" : "false") +
    ", \"humidity\": {\"sensor1\": " + String(sensor1percent) + 
    ", \"sensor2\": " + String(sensor2percent) + "}}";
    server.send(200, "application/json", json);
}