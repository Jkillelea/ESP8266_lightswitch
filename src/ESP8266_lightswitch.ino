#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "credentials.h"

// const char* ESP_HOSTNAME = "esp8266";
#define ESP_HOSTNAME "esp8266"

const int lightPin = 2;
ESP8266WebServer server(80);

void setup() {
  const int initalPinState = HIGH;
  const int startupDelay   = 3; // seconds

  pinMode(lightPin, OUTPUT);
  digitalWrite(lightPin, initalPinState);

  delay(startupDelay*1000);
  Serial.begin(115200);
  Serial.println("");

  configure_wifi();
  configure_server();
  configure_mdns();
}

void loop() {
  server.handleClient();
  delay(1); // sleep a little (trying to reduce current draw)
}

void configure_wifi() {
  WiFi.mode(WIFI_STA); // disconnect from any previous wifi station

  Serial.printf("[WiFi] connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  Serial.printf("[WiFi] connected to %s\n", WiFi.SSID().c_str());
  Serial.printf("[WiFi] IP address: %s\n", WiFi.localIP().toString().c_str());
}

void configure_mdns() {
  Serial.print("MDNS...");
  if (!MDNS.begin(ESP_HOSTNAME)) { // returns true if begun successfully
    Serial.println(" failed");
  } else {
    MDNS.addService("http", "tcp", 80); // Add service to MDNS-SD
    // Serial.println(" hostname " + String(ESP_HOSTNAME) + ".local");
    Serial.printf(" hostname %s.local\n", ESP_HOSTNAME);
  }
}

void configure_server() {
  Serial.print("Server...");
  server.begin();
  server.on("/", espToggleLight);
  server.on("/get", espGetLight);
  Serial.println(" on " + WiFi.localIP().toString());
}

void espToggleLight() {
  int state = readLightState() ? LOW : HIGH; // If currently high, then low
  digitalWrite(lightPin, state);
  server.send(200, "text/plain", String(state));
}

void espGetLight() {
  int state = readLightState();
  server.send(200, "text/plain", String(state));
}

void espWriteLight(int state) {
  digitalWrite(lightPin, state);
}

int readLightState() {
  return digitalRead(lightPin);
}
