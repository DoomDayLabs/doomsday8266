
#include "Doomsday8266.hpp"

IntSensor* temperature = new IntSensor("TEMPERATURE",0,100);
IntSensor* waterLevel = new IntSensor("LEVEL",0,3);
ValSensor<3>* state = new ValSensor<3>("STATE","STANDBY","HEAT","BOIL");
Trigger<0>* standby = new Trigger<0>("STANDBY");
Trigger<0>* boil = new Trigger<0>("BOIL");
Trigger<1>* heat = new Trigger<1>("HEAT",new IntParam(20,95));

void onBoil(TArg args){
  state->set(2);
}

void onStandby(TArg args){
  state->set(0);
}

void onHeat(TArg args){
  state->set(1);
}
void setup(Endpoint* e) {
  standby->on(onStandby);
  boil->on(onBoil);
  heat->on(onHeat);
  e->addSensor(temperature);
  e->addSensor(waterLevel);
  e->addSensor(state);  
  e->addTrigger(standby);
  e->addTrigger(boil);
  e->addTrigger(heat);
  
}
long t = 0;
int temp = 0;
void loop(Endpoint* e) {
  long now = millis();
  if (now-t>2000){
   // temperature->set(temp);
    temp++;
    t = now;
  }
}



