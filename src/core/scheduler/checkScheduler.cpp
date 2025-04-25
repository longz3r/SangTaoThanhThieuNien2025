#include "main.h"

void checkScheduler() {
    // Check for new schedules to activate

    static uint8_t lastMinute = 61;
    
    // Check active schedules
    for (auto& schedule : schedules) {
        if (schedule.isActive) {
        if (schedule.type == CONDITIONAL) {
            // Check if conditions are met
            bool conditionMet = schedule.requireBothSensors ?
                (sensor1percent >= schedule.sensor1Threshold && 
                sensor2percent >= schedule.sensor2Threshold) :
                (sensor1percent >= schedule.sensor1Threshold || 
                sensor2percent >= schedule.sensor2Threshold);
            
            if (conditionMet) {
                schedule.isActive = false;
                schedule.callback(false, schedule.pumpNumber);
                Serial.print("Condition met for schedule: ");
                Serial.println(schedule.description);
            }
        }
        else if (schedule.type == TIMED) {
            // Check if duration has elapsed
            if (millis() - schedule.activeSince >= schedule.durationSeconds * 1000) {
                schedule.isActive = false;
                schedule.callback(false, schedule.pumpNumber);
                Serial.print("Duration ended for schedule: ");
                Serial.println(schedule.description);
            }
        }
        }
    }

    if (now.minute() == lastMinute) return;
    lastMinute = now.minute();

    for (auto& schedule : schedules) {
        if (schedule.enabled && 
            !schedule.isActive &&
            schedule.hour == now.hour() && 
            schedule.minute == now.minute()) {
        
        schedule.isActive = true;
        schedule.activeSince = millis();
        schedule.callback(true, schedule.pumpNumber);
        Serial.print("Activated schedule: ");
        Serial.println(schedule.description);
        }
    }
}