#include <main.h>

void pumpStatusUpdate() {
    digitalWrite(12, pump1Status ? LOW : HIGH);
    digitalWrite(13, pump2Status ? LOW : HIGH);
}