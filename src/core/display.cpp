#include "main.h"

void displayStatusWithWiFi(float sensor1percent, float sensor2percent) {
    display.clearDisplay();
    display.setCursor(0, 0);
    wl_status_t status = WiFi.status();
    switch (status) {
      case WL_CONNECTED:
        display.println("Da ket noi WiFi");
        break;
      case WL_NO_SSID_AVAIL:
        display.println("Khong tim thay WiFi");
        break;
      case WL_CONNECT_FAILED:
        display.println("Ket noi that bai");
        break;
      case WL_IDLE_STATUS:
        display.println("Trang thai cho");
        break;
      case WL_DISCONNECTED:
        display.println("Mat ket noi WiFi");
        break;
      default:
        display.println("Khong xac dinh");
        break;
    }

    display.print(daysOfTheWeek[now.dayOfTheWeek()]);
    display.print(" - ");
    if(now.day() < 10) display.print('0');
    display.print(now.day(), DEC);
    display.print('/');
    // if(now.month() < 10) display.print('0');
    display.print(now.month(), DEC);
    display.print(" - ");
    if(now.hour() < 10) display.print('0');
    display.print(now.hour(), DEC);
    display.print(':');
    if(now.minute() < 10) display.print('0');
    display.print(now.minute(), DEC);
    display.print(':');
    if(now.second() < 10) display.print('0');
    display.print(now.second(), DEC);

    display.setCursor(0, 16);
    display.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    display.println("Bom 1");
    display.println("Bom 2");
    if (pin1Status) {
      display.drawBitmap(40, 24, tick, 8, 8, WHITE);
    } else {
      display.drawBitmap(40, 24, cross, 8, 8, WHITE);
    }
    if (pin2Status) {
      display.drawBitmap(40, 32, tick, 8, 8, WHITE);
    } else {
      display.drawBitmap(40, 32, cross, 8, 8, WHITE);
    }
  
    display.printf("Do am: %.1f%% - %.1f%%\n", sensor1percent, sensor2percent) ;
    display.printf("Nhiet do: %.2f%  CC", rtc.getTemperature());
    display.drawBitmap(90, 48, degree, 8, 8, WHITE);

    display.display();
    delay(50);
  }

void displayStatusWithoutWiFi(float sensor1percent, float sensor2percent) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Dang phat WiFi");
  display.printf("Ten: %s\n", AP_SSID);
  display.printf("MK: %s\n", AP_PASS);
  display.printf("IP: %s\n", WiFi.softAPIP().toString().c_str());
  display.println("Bom 1");
  display.println("Bom 2");
  if (pin1Status) {
    display.drawBitmap(40, 32, tick, 8, 8, WHITE);
  } else {
    display.drawBitmap(40, 32, cross, 8, 8, WHITE);
  }
  if (pin2Status) {
    display.drawBitmap(40, 40, tick, 8, 8, WHITE);
  } else {
    display.drawBitmap(40, 40, cross, 8, 8, WHITE);
  }
  display.printf("Do am: %.1f%% - %.1f%%\n", sensor1percent, sensor2percent) ;
  display.printf("Nhiet do: %.2f%  CC", rtc.getTemperature());
  display.drawBitmap(128, 56, degree, 8, 8, WHITE);
  display.display();
  delay(50);
}

void displayMessage(String message, int time) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(message);
  display.display();
  delay(time);
}

void updateDisplay() {
  now = rtc.now();

  int sensor1mV = constrain(analogReadMilliVolts(32), 200, 2200);
  sensor1percent = map(sensor1mV, 2200, 200, 0, 1000);
  sensor1percent = sensor1percent / 10;

  int sensor2mV = constrain(analogReadMilliVolts(33), 2000, 3000);
  sensor2percent = map(sensor2mV, 2000, 3000, 0, 1000);
  sensor2percent = sensor2percent / 10;

  if (wifiConfigured) {
    displayStatusWithWiFi(sensor1percent, sensor2percent);
  } else {
    displayStatusWithoutWiFi(sensor1percent, sensor2percent);
  }
}