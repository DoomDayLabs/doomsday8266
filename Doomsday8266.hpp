#define DOOMSDAYINO_USE_AS_LIB
#include "DoomsDayIno.h"
#include "captive_portal.hpp"
#include "wifi.hpp"

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

#include <ArduinoJson.h>
#include "FS.h"



typedef struct Config {
  const char* ssid;
  const char* pass;
  const char* pincode;
} Config;

void loadsConfig(Config* cfg) {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return ;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return ;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return ;
  }

  cfg->ssid = json["ssid"];
  cfg->pass = json["pass"];
  cfg->pincode = json["pincode"];
  
//  const char* serverName = json["serverName"];
//  const char* accessToken = json["accessToken"];

  // Real world application would store these values in some variables for
  // later use.

  Serial.print("Loaded serverName: ");
//  Serial.println(serverName);
  Serial.print("Loaded accessToken: ");
//  Serial.println(accessToken);
//  return true;
}



Config loadConfig() {
  Config cfg;
  cfg.ssid = "TEST";
  cfg.pass = "TEST";
  cfg.pincode = "12345678";
  return cfg;
}

void setup(Endpoint* e);
void loop(Endpoint* e);

typedef void (*Setup)();
typedef void (*Loop)();

CaptivePortal* portal;

void setup_maint() {  
  portal = new CaptivePortal();
  portal->setup();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop_maint() {
  portal->loop();
}

Endpoint* endpoint;
Protocol* proto;
Esp* esp;


Config cfg;
void setup_prod() {
  digitalWrite(LED_BUILTIN, LOW);

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;    
  }
  
  cfg = loadConfig();
  loadsConfig(&cfg);
  endpoint = new Endpoint();
  endpoint->setPin(cfg.pincode);  
  proto = new Protocol(endpoint, NULL);
  esp = new Esp(cfg.ssid, cfg.pass);
  esp->setup(endpoint, proto);
  setup(endpoint);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop_prod() {
  esp->loop(endpoint, proto);
  proto->read();
  loop(endpoint);
  proto->write();
  
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  
}

Setup fSetup;
Loop fLoop;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  const rst_info * info = system_get_rst_info();
  int rst_reason = info->reason;
  Serial.printf("Reason %d\r\n",rst_reason);
  if (rst_reason == 0) {
    fSetup = setup_maint;
    fLoop = loop_maint;
  } else {
    fSetup = setup_prod;
    fLoop = loop_prod;
  }

  fSetup();

}


void loop() {
  fLoop();
}

