#include "main.h"

bool addConditionalSchedule(
    uint8_t pumpNumber,
    uint8_t hour, 
    uint8_t minute, 
    void (*callback)(bool, uint8_t), 
    String desc,
    float sensor1Limit, 
    float sensor2Limit,
    bool bothSensorsRequired = true,
    bool enabled = true
  ) {
    if (hour > 23 || minute > 59 || pumpNumber < 1) return false;
    
    Schedule newSchedule = {
      pumpNumber,
      CONDITIONAL,
      hour,
      minute,
      callback,
      desc,
      enabled,
      sensor1Limit,
      sensor2Limit,
      bothSensorsRequired,
      0,  // duration not used
      false,
      0
    };
    
    schedules.push_back(newSchedule);
    return true;
  }

bool addTimedSchedule(
    uint8_t pumpNumber,
    uint8_t hour,
    uint8_t minute,
    void (*callback)(bool, uint8_t), 
    String desc,
    unsigned long durationMinutes,
    bool enabled = true
  ) {
    if (hour > 23 || minute > 59 || pumpNumber < 1) return false;
    
    Schedule newSchedule = {
      pumpNumber,
      TIMED,
      hour,
      minute,
      callback,
      desc,
      enabled,
      0, 0, false,  // sensor thresholds not used
      durationMinutes,
      false,
      0
    };
    
    schedules.push_back(newSchedule);
    return true;
 }

 void pumpControl(bool state, uint8_t pumpNumber) {
    if (pumpNumber == 1) {
        pump1Status = state;
    } else if (pumpNumber == 2) {
        pump2Status = state;
    } else if (pumpNumber == 3) {
        pump1Status = state;
        pump2Status = state;
    }
}