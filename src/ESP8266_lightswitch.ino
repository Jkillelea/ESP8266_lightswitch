#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* HOME_SSID     = "ATT9Iej6KX";
const char* HOME_PASSWORD = "2=3es5pcw7pf";

const int lightPin = 2;
ESP8266WebServer server(80);

void setup() {
  const int startupDelay = 3; // seconds
  const int initalPinState = LOW;

  delay(startupDelay*1000);
  Serial.begin(115200);
  pinMode(lightPin, OUTPUT);
  Serial.println("");
  configure_wifi();
  configure_server();
  configure_mdns();

  digitalWrite(lightPin, initalPinState);
}

void loop() {
  server.handleClient();
  delay(1);
}

void configure_wifi() {
  Serial.print("WiFi...");
  WiFi.mode(WIFI_STA); // disconnect from any previous wifi station
  WiFi.begin(HOME_SSID, HOME_PASSWORD);
  Serial.println(" started");
}

void configure_mdns() {
  Serial.print("MDNS...");
  if (!MDNS.begin("esp8266")) { // returns true if begun successfully
    Serial.println(" failed");
  } else {
    MDNS.addService("http", "tcp", 80); // Add service to MDNS-SD
    Serial.println(" started");
  }
}

void configure_server() {
  Serial.print("Server...");
  server.begin();
  server.on("/", espToggleLight);
  Serial.print(" on ");
  Serial.println(WiFi.localIP().toString());
}

void espToggleLight() {
  int state = readLightState() ? LOW : HIGH; // If currently high, then low
  // String response = String(state);
  digitalWrite(lightPin, state);
  server.send(200, "text/plain", String(state));
  // Serial.println(response);
}

void espWriteLight(int state) {
  digitalWrite(lightPin, state);
}

int readLightState() {
  return digitalRead(lightPin);
}
