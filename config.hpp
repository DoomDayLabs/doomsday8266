//#include <ArduinoJson.h>
//#include "FS.h"
typedef struct Config {
  const char* ssid;
  const char* pass;
  const char* pincode;
} Config;


/*
void loadConfig(Config* cfg) {    
  
  cfg->ssid = "WIFI";
  cfg->pass = "PASSWD";
  cfg->pincode = "12345678";
  
  
  Serial.println("Mount FS");
  if (!SPIFFS.begin()) {
      Serial.println("Failed to mount file system");
      return;   
  }
  delay(100);

  Serial.println("Open CFG");
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return;
  }
  
  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return;
  }

  // Allocate a buffer to store contents of the file.
  //std::unique_ptr<char[]> buf(new char[size]);
  char buffer[size];
  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  Serial.println("Read CFG");
  configFile.readBytes(buffer, size);

  StaticJsonBuffer<200> jsonBuffer;
  Serial.println("Parse CFG");
  JsonObject& json = jsonBuffer.parseObject(buffer);

  if (!json.success()) {
    //Serial.println("Failed to parse config file");
    return;
  }

  cfg->ssid = json["ssid"];
  cfg->pass=json["pass"];
  cfg->pincode=json["pincode"];

  
}
*/
