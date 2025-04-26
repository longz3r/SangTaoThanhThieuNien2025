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
    saveSchedule(schedules);
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
    saveSchedule(schedules);
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

std::vector<Schedule> loadStoredSchedules() {
  std::vector<ScheduleStorage> loadedSchedules;
  size_t count = config.getUInt("schedules_count", 0);
  std::vector<Schedule> schedulesNeedLoading;

  if (count > 0) {
    loadedSchedules.resize(count);
    config.getBytes("schedules", loadedSchedules.data(), count * sizeof(ScheduleStorage));
    
    for (const auto& ss : loadedSchedules) {
      Schedule s;
      s.pumpNumber = ss.pumpNumber;
      s.type = static_cast<ScheduleType>(ss.type);
      s.hour = ss.hour;
      s.minute = ss.minute;
      s.description = String(ss.description);
      s.enabled = ss.enabled;
      s.sensor1Threshold = ss.sensor1Threshold;
      s.sensor2Threshold = ss.sensor2Threshold;
      s.requireBothSensors = ss.requireBothSensors;
      s.durationSeconds = ss.durationSeconds;

      s.callback = pumpControl;  // <-- Restore callback manually
      s.isActive = false;
      s.activeSince = 0;

      schedulesNeedLoading.push_back(s);
    }
  }

  return schedulesNeedLoading;

}

ScheduleStorage fromSchedule(const Schedule& s) {
  ScheduleStorage ss;
  ss.pumpNumber = s.pumpNumber;
  ss.type = static_cast<uint8_t>(s.type);
  ss.hour = s.hour;
  ss.minute = s.minute;
  s.description.toCharArray(ss.description, sizeof(ss.description));  // safe copy
  ss.enabled = s.enabled;
  ss.sensor1Threshold = s.sensor1Threshold;
  ss.sensor2Threshold = s.sensor2Threshold;
  ss.requireBothSensors = s.requireBothSensors;
  ss.durationSeconds = s.durationSeconds;
  return ss;
}

void saveSchedule(std::vector<Schedule> schedules) {
  std::vector<ScheduleStorage> savedSchedules;
    for (const auto& sched : schedules) {
      savedSchedules.push_back(fromSchedule(sched));
    }

    config.putBytes("schedules", savedSchedules.data(), savedSchedules.size() * sizeof(ScheduleStorage));
    config.putUInt("schedules_count", savedSchedules.size());
}