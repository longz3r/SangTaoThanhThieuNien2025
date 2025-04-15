#include "Arduino.h"
#include "Wire.h"
#include "SPI.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include <WiFi.h>
#include <WebServer.h>

// Config
Adafruit_SSD1306 display(128, 64, &Wire, -1);
const char* ssid = "VNPT VIET HA 4G";
const char* pass = "08092008";

bool pin1Status = false;
bool pin2Status = false;

WebServer server(80);

const unsigned char tick[] PROGMEM = {
  0b00000000,
  0b00000001,
  0b00000010,
  0b00000100,
  0b10001000,
  0b01010000,
  0b00100000,
  0b00000000
};

const unsigned char cross [] PROGMEM = {
  0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81
};


String generateHTML() {
  String color1 = pin1Status ? "green" : "red";
  String label1 = pin1Status ? "BẬT" : "TẮT";

  String color2 = pin2Status ? "green" : "red";
  String label2 = pin2Status ? "BẬT" : "TẮT";

  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body { text-align: center; font-family: sans-serif; margin-top: 50px; }
        .button {
          font-size: 24px;
          padding: 20px 40px;
          margin: 20px;
          color: white;
          border: none;
          border-radius: 10px;
          cursor: pointer;
        }
      </style>
    </head>
    <body>
      <h1>Điều khiển</h1>
      <button class="button" id="btn1" style="background-color: )rawliteral" + color1 + R"rawliteral(;") onclick="toggleGPIO(1)">Pump 1 - )rawliteral" + label1 + R"rawliteral(</button>
      <button class="button" id="btn2" style="background-color: )rawliteral" + color2 + R"rawliteral(;") onclick="toggleGPIO(2)">Pump 2 - )rawliteral" + label2 + R"rawliteral(</button>

      <script>
        function toggleGPIO(pin) {
          fetch('/toggle?pin=' + pin)
            .then(response => response.text())
            .then(data => {
              document.open();
              document.write(data);
              document.close();
            });
        }
      </script>
    </body>
    </html>
  )rawliteral";

  return html;
}

// Variable to store the HTTP request
// Request Handlers
void handleRoot() {
  server.send(200, "text/html", generateHTML());
}

void handleToggle() {
  if (server.hasArg("pin")) {
    int pin = server.arg("pin").toInt();
    if (pin == 1) {
      pin1Status = !pin1Status;
      digitalWrite(2, pin1Status ? HIGH : LOW);
    } else if (pin == 2) {
      pin2Status = !pin2Status;
      digitalWrite(4, pin2Status ? HIGH : LOW);
    }
  }
  server.send(200, "text/html", generateHTML());
}


void displayStatus() {
  display.clearDisplay();
  display.setCursor(0, 0);
  wl_status_t status = WiFi.status();
  switch (status) {
    case WL_CONNECTED:
      display.println("Da ket noi WiFi");
      break;
    case WL_NO_SSID_AVAIL:
      display.println("No SSID Available");
      break;
    case WL_CONNECT_FAILED:
      display.println("Ket noi that bai");
      break;
    case WL_IDLE_STATUS:
      display.println("Idle");
      break;
    case WL_DISCONNECTED:
      display.println("Mat ket noi");
      break;
    default:
      display.println("Khong xac dinh");
      break;
  }
  display.printf("%s (%ld)\n", WiFi.SSID().c_str(), WiFi.RSSI());
  display.printf("IP: %s\n", WiFi.localIP().toString().c_str());
  // display.printf("Pump 1: %s\n", pin1Status ? "ON" : "OFF");
  // display.printf("Pump 2: %s\n", pin2Status ? "ON" : "OFF");
  display.println("Pump 1");
  display.println("Pump 2");
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
  display.display();
}

void setup() {
  // Config 
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Display intro
  display.display();
  delay(200);

  // Wifi connection
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Dang ket noi WiFi");
  display.display();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    display.print(".");
    display.display();
  }

  server.on("/", handleRoot);
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
}

void loop() {
  displayStatus();
  server.handleClient();
}