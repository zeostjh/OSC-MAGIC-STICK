#include <WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>

const int hallPin = 2 ;
const char * ssid = "?*****?";
const char * pwd = "?*****?";
const IPAddress Destination_Address(0,0,0,0);
const int udpPort = 8888;//destination port
IPAddress ip(0,0,0,0), gateway(0,0,0,0), subnet(255,255,255,0);
int val = 0;
boolean connected = false;
WiFiUDP udp;


void setup()
{
  pinMode ( hallPin , INPUT ) ;
  Serial.begin(115200);
  connectToWiFi(ssid, pwd);
}

void loop()
{
  if(connected)
  Serial.println('NETWORK CONNECTED and SENSOR RUNNING');
  val = hallRead();
  OSCMessage msg;
  {
    if(val >= 20)
    {
        Serial.print('ON =');
        Serial.println(val);
        OSCMessage msg("/esp/ID_001/1");
        udp.beginPacket(Destination_Address, udpPort);
        msg.send(udp); 
        udp.endPacket();
        msg.empty();
    }
    delay(200);
    if(val <= 19)
    {
        Serial.print('OFF =');
        Serial.println(val);
        OSCMessage msg("/esp/ID_001/0");
        udp.beginPacket(Destination_Address, udpPort);
        msg.send(udp); 
        udp.endPacket();
        msg.empty();
      }
      delay(200);
      }
  }
void connectToWiFi(const char * ssid, const char * pwd)
{
  Serial.println("Connecting to Network: " + String(ssid));
  WiFi.disconnect(true);
  WiFi.onEvent(WiFiEvent);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pwd);
  Serial.println("Waiting for WIFI connection...");
}
void WiFiEvent(WiFiEvent_t event)
{
    switch(event)
    {
      case SYSTEM_EVENT_STA_GOT_IP:
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
    }
}
