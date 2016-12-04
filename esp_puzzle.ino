#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer webServer(80);

const char responseHTML[] PROGMEM = ""
  "<!DOCTYPE html><html><head><title>ESP8266 Puzzle</title></head><body>"
  "<h1>Oops!</h1><p>You seem to have reached an invalid page!</p></body></html>";

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("ESP8266_Puzzle");

  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", apIP);

  // replay to all requests with same HTML
  webServer.onNotFound([]() {
    webServer.send_P(200, "text/html", responseHTML);
  });
  webServer.begin();
  phase1_setup();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
  phase1_loop();
}
