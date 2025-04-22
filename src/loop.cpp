// loop.cpp
#include "main.h"

void loop() {
  int sensor1mV = constrain(analogReadMilliVolts(32), 200, 2200);
  float sensor1percent = map(sensor1mV, 2200, 200, 0, 1000);

  int sensor2mV = constrain(analogReadMilliVolts(33), 2000, 3000);
  float sensor2percent = map(sensor2mV, 2000, 3000, 0, 1000);

  displayStatus(sensor1percent / 10, sensor2percent / 10);

  if (sensor1percent / 10 < 40) {
    digitalWrite(12, LOW);
    pin1Status = true;
  } else {
    digitalWrite(12, HIGH);
    pin1Status = false;
  }
  server.handleClient();
}