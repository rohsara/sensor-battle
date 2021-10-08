#define WIFI_SSID "replace with your wifi"
#define WIFI_PASS "replace with your wifi pwd"

#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>

// Wifi
int status = WL_IDLE_STATUS;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

// Server address & port
IPAddress serverAddress(104, 248, 12, 114);
unsigned int serverPort = 33333;
unsigned int localPort = 2390;
WiFiUDP Udp;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);
  setupWifi();
  Udp.begin(localPort);
}

void loop() {
  if (digitalRead(2) == LOW) {
    sendMessage("s.fire");
  }

  int sensor1 = analogRead(A0);
  delay(1);
  int sensor2 = analogRead(A1);

  int xAxis = map(sensor1, 0, 1023, -10, 10);
  int yAxis = map(sensor2, 0, 1023, -10, 10);

  if (yAxis >= 5) {
    sendMessage("s.left");
  }
  if (yAxis <= -5) {
    sendMessage("s.right");
  }
  if (xAxis >= 5) {
    sendMessage("s.back");
  }
  if (xAxis <= -5) {
    sendMessage("s.go");
  }

  Serial.print(xAxis);
  Serial.print(" ");
  Serial.println(yAxis);

  delay(50);
}

void sendMessage(char* message) {
  // send a UDP message to server
  Udp.beginPacket(serverAddress, serverPort);
  Udp.write(message);
  Udp.endPacket();

  Serial.println("Sent!");
}

void setupWifi() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
  }

  // report and turn on the builtin LED when connected:
  Serial.println("Connected to wifi");
  digitalWrite(LED_BUILTIN, HIGH);

  printWifiStatus();
}

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
