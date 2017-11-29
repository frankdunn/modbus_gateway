#include <ESP8266WiFi.h>
#include <WiFiClient.h>

// Hardcode WiFi parameters as this isn't going to be moving around.
const char* ssid = "frank1";
const char* password = "annarieannarie";

// Start a TCP Server on port 5045
WiFiServer server(502);
WiFiClient client ;

size_t len = 50;
uint8_t sbuf[50];

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  Serial.println("");
  //Wait for connection
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
 
  // Start the TCP server
  server.begin();
  
}

void loop() {
  
    
  
    if (!client.connected()) {
      client = server.available();
    }
    else{     
    
    if (int readnum2 = client.available() > 0) {
      // Read incoming message
      memset(sbuf, 0, 50);
      int readnum = client.read(sbuf,len);
     
     
      // Send back something to the clinet
      server.write(sbuf,len);
      // Echo input on Serial monitor
      //Serial.write(sbuf,len);
      for (int i=0 ;i<readnum;i++){
      Serial.print(sbuf[i],HEX);
      }
      Serial.print("\n\r");
      Serial.println(readnum2);
    }
  }
 }

