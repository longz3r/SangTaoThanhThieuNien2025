#include "main.h"

Sensor readHumidity() {
    return Sensor{
        static_cast<float>(sensor1percent / 10.0),
        static_cast<float>(sensor2percent / 10.0)
    };
}