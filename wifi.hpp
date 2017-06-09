#include <ESP8266WiFi.h>
#include "discovery.hpp"



class Esp {
  private:
    const char* ssid;
    const char* pass;    
    bool serverStarted = false;
    WiFiServer server = WiFiServer(27015);
    WiFiClient client;
    DiscoverySender discover;
    void startServer() {
      server.begin();
      serverStarted = true;
    }

    void stopServer() {
      server.stop();
      serverStarted = false;
    }
    void stopClient() {
      client.stop();
    }

    bool acceptConnection(Protocol* p) {
      WiFiClient c = server.available();
      if (c) {
        if (client.connected()) {
          c.println("BUSY");
          c.flush();
          c.stop();
        } else {
          client.stop();
          client = c;
          client.flush();
          //p->setStream(&client);
          return true;
        }
      }
      return false;
    }
  public:
    Esp(const char* ssid, const char* pass) {
      this->ssid = ssid;
      this->pass = pass;
    }

    void setup(Endpoint* e, Protocol* p) {
      WiFi.begin(ssid, pass);
    }

    void loop(Endpoint* e, Protocol* p) {
      if (WiFi.status() == WL_CONNECTED) {
        if (!discover.isReady()){
          discover.setup(WiFi.localIP(),e->getDevClass(),e->getDevSerial());
        }
        if (!serverStarted) {
          startServer();
        }

        if (acceptConnection(p)) {
          p->setStream(&client);
          e->state = 1;
        }

        if (!client.connected()) {
          client.stop();
          e->state = 0;
          p->setStream(NULL);
          discover.send();
        }

      } else {
        discover.reset();
        stopServer();
        stopClient();
        p->setStream(NULL);
      }
    }

    bool connected() {
      return client.connected();
    }
};


