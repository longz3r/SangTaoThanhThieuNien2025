#include "Arduino.h"
#include "Wire.h"
#include "SPI.h"
#include "Adafruit_SSD1306.h"
#include "Adafruit_GFX.h"
#include <WiFi.h>

// Enter your WiFi SSID and password
char ssid[] = "VNPT VIET HA 4G";             // your network SSID (name)
char pass[] = "08092008";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;                      // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)

char server[] = "wifitest.adafruit.com";    // name address for adafruit test
char path[]   = "/testwifi/index.html";

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

Adafruit_SSD1306 display(128, 64, &Wire, -1);
const unsigned char waterDropIcon [] PROGMEM = {
  0x00, 0x00,
  0x03, 0xC0,
  0x0F, 0xF0,
  0x1F, 0xF8,
  0x3F, 0xFC,
  0x3F, 0xFC,
  0x3F, 0xFC,
  0x1F, 0xF8,
  0x0F, 0xF0,
  0x07, 0xE0,
  0x03, 0xC0,
  0x01, 0x80,
  0x01, 0x80,
  0x00, 0x00,
  0x00, 0x00,
  0x00, 0x00
};

const unsigned char snowflakeBitmap [] PROGMEM = {
  0b00000000, 0b00000000,
  0b00010000, 0b00001000,
  0b00001000, 0b00010000,
  0b10000100, 0b00100001,
  0b01000010, 0b01000010,
  0b00100001, 0b10000100,
  0b00010000, 0b00001000,
  0b00001000, 0b00010000,
  0b00010000, 0b00001000,
  0b00100001, 0b10000100,
  0b01000010, 0b01000010,
  0b10000100, 0b00100001,
  0b00001000, 0b00010000,
  0b00010000, 0b00001000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000
};

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  // pinMode(2, OUTPUT);
  // pinMode(4, OUTPUT);

  // // Optional: turn both relays OFF initially (depending on relay logic)
  // digitalWrite(2, LOW); // or LOW depending on module
  // digitalWrite(4, HIGH);

  Serial.begin(500000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  // Serial.print("Attempting to connect to SSID: ");
  // Serial.println(ssid);

  // WiFi.begin(ssid, pass);
  // while (WiFi.status() != WL_CONNECTED) {
  //     delay(500);
  //     Serial.print(".");
  // }

  // Serial.println("");
  // Serial.println("Connected to WiFi");
  // printWifiStatus();

  // Serial.println("\nStarting connection to server...");
  // // if you get a connection, report back via serial:
  // if (client.connect(server, 80)) {
  //   Serial.println("connected to server");
  //   // Make a HTTP request:
  //   client.print("GET "); client.print(path); client.println(" HTTP/1.1");
  //   client.print("Host: "); client.println(server);
  //   client.println("Connection: close");
  //   client.println();
  // }


  Serial.begin(500000);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay(); // Clear the buffer.
  display.display();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.clearDisplay();
  display.println("==========");
  display.println("WELCOME");
  display.println("==========");
  display.display();
  delay(1000);

  // for (int i = 0; i <= 10; i++) {
  //   if (i % 2 == 0 ) {
  //     digitalWrite(2, LOW); // or LOW depending on module
  //     digitalWrite(4, HIGH);
  //   } else {
  //     digitalWrite(2, HIGH); // or LOW depending on module
  //     digitalWrite(4, LOW);
  //   }
  //   delay(1000);
  // }
}

void loop() {
  int sensorValue = analogRead(15);  // GPIO34
  int sensorValue2 = analogRead(32);  // GPIO34
  float voltage = sensorValue * (3.3 / 4095.0);
  float voltage2 = sensorValue2 * (3.3 / 4095.0);

  float moisturePercent = 100 - (sensorValue / 4095.0) * 100.0;
  float moisturePercent2 = 100 - (sensorValue2 / 4095.0) * 100.0;
  display.clearDisplay(); // Clear the buffer.

  display.setTextSize(1);
  display.setTextColor(WHITE);
  // display.clearDisplay();
  // display.println(String(sensorValue));
  // display.println("");
  // display.println(moisturePercent);
  display.drawBitmap(0, 0, waterDropIcon, 16, 16, WHITE);
  display.drawBitmap(0, 16, snowflakeBitmap, 16, 16, WHITE);
  display.setCursor(24, 0);
  display.println("LINH LON XAU XI");
  display.setCursor(0, 32);
  display.print("Gtri 1: ");
  display.print(voltage);
  display.println("V");

  display.print("Do am: ");
  display.print(moisturePercent, 2); // 1 decimal place
  display.println("%");
  display.print("Gtri 2: ");
  
  // display.print(sensorValue2);
  // display.print(" - ");
  display.print(voltage2);
  display.println("V");
  display.print("Do am 2: ");
  display.print(moisturePercent2, 2); // 1 decimal place
  display.println("%");
  display.display();
  delay(1000);

  // put your main code here, to run repeatedly:
}


