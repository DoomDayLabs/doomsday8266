#define DOOMSDAYINO_USE_AS_LIB
#include <DoomsDayIno.h>
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
  char* buff = (char*)(malloc(size));
  //std::unique_ptr<char[]> buf(new char[size]);
  
  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buff, size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buff);
  //memset(buf.get(),0,size);
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
  //Serial.println(buf.get());
  Serial.print("Loaded pincode ");
  Serial.println(strlen(cfg->pincode));
  Serial.println(cfg->pincode);
  Serial.print("Loaded ssid ");
  Serial.println(cfg->ssid);
  Serial.print("Loaded pass ");
  Serial.println(cfg->pass);
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
char* serial = "0000000000000000";
int chipId = ESP.getChipId();
int flashChipId = ESP.getFlashChipId();

void setup_prod() {
  endpoint = new Endpoint();
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    for (;;){
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
    }
    
  }
  sprintf(serial,"%X%X",chipId,flashChipId);
  digitalWrite(LED_BUILTIN, LOW);

  //delay(100);

  cfg = loadConfig();
  loadsConfig(&cfg);
  
  
  endpoint->setDevSerial(serial);
  endpoint->setDevClass("DOOMDAYDEVICE");
  proto = new Protocol(endpoint, NULL);
  esp = new Esp(cfg.ssid, cfg.pass);
  esp->setup(endpoint, proto);
  setup(endpoint);
  digitalWrite(LED_BUILTIN, HIGH);
  endpoint->setPin(cfg.pincode);
}

void loop_prod() {
  esp->loop(endpoint, proto);
  proto->read();
  loop(endpoint);
  proto->write();
  if (esp->wifiActive()) {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    delay(10);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(10);
  }

}

Setup fSetup;
Loop fLoop;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  const rst_info * info = system_get_rst_info();
  int rst_reason = info->reason;
  Serial.printf("Reason %d\r\n", rst_reason);
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

