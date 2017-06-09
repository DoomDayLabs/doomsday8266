#include "Doomsday8266.hpp"

void setup(Endpoint* e) {

}
void loop(Endpoint* e) {

}

/*
#include "DoomsDayIno.h"
#include "wifi.hpp"
#include "captive_portal.hpp"

//#include <ESP8266WiFi.h>

const char* ssid = "DIGISKY";
const char* pass = "D1G1onAir";
CaptivePortal portal = CaptivePortal();
Esp esp(ssid, pass);

/*
IntSensor sens1("TEMPERATURE",0,100);
ValSensor<3>* stateSensor = new ValSensor<3>("STATE","STANDBY","HEAT","BOIL");
Trigger<0> standby("STANDBY");
Trigger<0> boil("BOIL");


void onBoil(TArg args){
  stateSensor->set(2);
}

void onStandby(TArg args){
  stateSensor->set(0);
}


void setup(Endpoint* e, Protocol* p) {
  
  pinMode(LED_BUILTIN, OUTPUT);
  e->setPin("111");
  e->setDevClass("DDD.DIY.TEST");
  e->setDevSerial("00001");  
  esp.setup(e, p);
  
}

void loop(Endpoint* e, Protocol* p) {
  portal.loop();
  esp.loop(e, p);
  if (esp.connected()) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level                                   // but actually the LED is on; this is because                                     // it is acive low on the ESP-01)
    delay(100);                      // Wait for a second
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(200);
  }


}
*/

