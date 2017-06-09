#define DOOMSDAYINO_USE_AS_LIB
#include "DoomsDayIno.h"
#include "captive_portal.hpp"
#include "wifi.hpp"

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif


typedef struct Config {
  char* ssid;
  char* pass;
  char* pincode;
} Config;


Config loadConfig() {
  Config cfg;
  cfg.ssid = "DIGISKY";
  cfg.pass = "D1G1onAir";
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
  cfg = loadConfig();
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

