#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
class DiscoverySender {
  private:
    bool ready = false;
    char* discoveryPacket = NULL;
    IPAddress mcastIp = IPAddress(239, 141, 12, 12);
    IPAddress localIp;
    long lastSendDiscovery = 0;
    WiFiUDP udp;
  public:
    bool isReady() {
      return ready;
    };

    void setup(IPAddress ip, const char* devClass, const char* devId) {
      char c_desc[1024];
      sprintf(c_desc, "%s %s DEVICE", devClass, devId);
      if (discoveryPacket != NULL)
        free(discoveryPacket);
      discoveryPacket = (char*)(malloc(strlen(c_desc)));
      strcpy(discoveryPacket, c_desc);
      localIp = ip;
      //udp.beginMulticast(localIp,mcastIp,27015);      
      ready = true;
      
      
    };

    void reset() {
      ready = false;      
    };


    void send() {
      long t = millis();
      if (t-lastSendDiscovery<1000)
        return;

      udp.beginPacketMulticast(mcastIp,27015,localIp);
      udp.write(discoveryPacket,strlen(discoveryPacket));
      udp.endPacket();

      lastSendDiscovery = t;
      
             
    }
};

