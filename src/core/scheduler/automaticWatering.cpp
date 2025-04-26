#include <main.h>

void automaticWatering() {
    if (automaticWateringStatus) {
        if (sensor1percent < sensor1Lower) {
            pump1Status = true;
        } else if (sensor1percent > sensor1Upper) {
            pump1Status = false;
        }
        if (sensor2percent < sensor2Lower) {
            pump2Status = true;
        } else if (sensor2percent > sensor2Upper) {
            pump2Status = false;
        }
    }
}