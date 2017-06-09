const char* html =
#include "portal.html.h"
  ;
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include "FS.h"



class CaptivePortal {
  private:
    const char* ssid = "DOOM DAY DEVICE";
    const byte DNS_PORT = 53;
    IPAddress apIP = IPAddress(192, 168, 1, 1);
    DNSServer dnsServer;
    ESP8266WebServer server = ESP8266WebServer(80);
    char* defaultConfig = "{\"pincode\":\"12345678\"}";


    int loadConfig(char* buffer) {
      File configFile = SPIFFS.open("/config.json", "r");
      if (!configFile) {
        strcpy(defaultConfig, buffer);
        return strlen(defaultConfig);
      }
      size_t size = configFile.size();
      configFile.readBytes(buffer, size);
      return size;
    }

    void saveConfig(char* conf) {
      File configFile = SPIFFS.open("/config.json", "w");
      if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return;
      }
      configFile.print(conf);
    }
  public:
    void setup() {
      CaptivePortal* self = this;
      if (!SPIFFS.begin()) {
        Serial.println("Failed to mount file system");
        return;
      }

      WiFi.mode(WIFI_AP);
      WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
      WiFi.softAP(ssid, "");
      dnsServer.start(DNS_PORT, "*", apIP);

      server.on("/", HTTP_GET, [self]() {
        self->server.sendHeader("Connection", "close");
        self->server.sendHeader("Acces-Control-Allow-Origin", "*");
        self->server.send(200, "text/html", html);
      });


      server.on("/load", HTTP_GET, [self]() {
        char buffer[1024];
        int size = self->loadConfig(buffer);
        buffer[size] = 0;
        self->server.sendHeader("Connection", "close");
        self->server.sendHeader("Acces-Control-Allow-Origin", "*");
        self->server.send(200, "text/html", buffer);
      });

      server.on("/save", HTTP_GET, [self]() {
        self->server.sendHeader("Connection", "close");
        char cfg[1024];
        self->server.arg("cfg").toCharArray(cfg, 1024);
        self->saveConfig(cfg);
        self->server.send(200, "text/html", "{}");
      });

      server.on("/reboot", HTTP_GET, []() {
        Serial.println("REBOOT");
        ESP.restart();
      });

      server.onNotFound([self]() {
        
        self->server.send(200, "text/html", html);
      });

      

      server.begin();
    }
    void loop() {
      
//      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(LED_BUILTIN, HIGH);
      dnsServer.processNextRequest();
      server.handleClient();
    }
};

