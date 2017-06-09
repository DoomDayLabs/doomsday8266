
#include "Doomsday8266.hpp"

IntSensor* temperature = new IntSensor("TEMPERATURE",0,100);
IntSensor* waterLevel = new IntSensor("LEVEL",0,3);
ValSensor<3>* state = new ValSensor<3>("STATE","STANDBY","HEAT","BOIL");

void setup(Endpoint* e) {
  e->addSensor(temperature);
  e->addSensor(waterLevel);
  e->addSensor(state);
//  e->setPin("111");
}
void loop(Endpoint* e) {

}



