// RSS Documentation: https://validator.w3.org/feed/docs/rss2.html

#include "WiFi.h"
#include <HTTPClient.h>
#include <tinyxml2.h>
 
using namespace tinyxml2;

const char* ssid = "Glendower Place";
const char* password = "Gl3nDower";
const char* named;

String hostname = "News Reader";
String responseBody;

XMLDocument rssFeed;
XMLElement * channel;
XMLElement * title;
XMLElement * description;
XMLElement * currentItem;
XMLNode * rss;


void setup() 
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    initWiFi();
    delay(100);

//    WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_STA_CONNECTED);
//    WiFi.onEvent(WiFiGotIP, SYSTEM_EVENT_STA_GOT_IP);
//    WiFi.onEvent(WiFiStationDisconnected, SYSTEM_EVENT_STA_DISCONNECTED);

    Serial.println("Setup done");
}

void loop() {
  // put your main code here, to run repeatedly:
  
  String rssPath = "http://feeds.bbci.co.uk/news/world/rss.xml";
  responseBody = httpGETRequest(rssPath.c_str());

  rssFeed.Parse(responseBody.c_str());

  Serial.println("I got past parse!");
 
  rss = rssFeed.LastChild(); // RSS specs mean that the rss node should always be last
  
  channel = rss->FirstChildElement(); // channel is the only child of rss
  Serial.println("I got past channel!");
  
  itemCarousel(channel);

}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void itemCarousel(XMLElement * channelElement) {

  for(XMLElement * loopItem = channelElement->FirstChildElement("item"); loopItem; loopItem = loopItem->NextSiblingElement("item") )
  {
    title = loopItem->FirstChildElement();
    description = loopItem->FirstChildElement("description");
    Serial.println(title->GetText());
    delay(1000);
    Serial.println(description->GetText());
    delay(2000);
  }
  
}
