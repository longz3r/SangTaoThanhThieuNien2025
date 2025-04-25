#include <main.h>

void automaticWatering() {
    if (automaticWateringStatus) {
        if (sensor2percent < 20) {
            pump1Status = true;
        } else {
            pump1Status = false;
        }
    }
}