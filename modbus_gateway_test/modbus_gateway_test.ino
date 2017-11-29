#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

const char* ssid = "frank1";
const char* password = "annarieannarie";

// Start a TCP Server on port 502

WiFiServer server(502);
WiFiClient client ;

size_t len = 50;
uint8_t sbuf[256];
uint8_t rtu_buf[50];
uint8_t rtu_len;

uint16_t calcCRC(uint8_t u8_buff_size)
{
  uint32_t tmp, tmp2, flag;
  tmp = 0xFFFF;

  for (uint8_t i = 0; i < u8_buff_size; i++)
  {
    tmp = tmp ^ rtu_buf[i];
      for (uint8_t j = 1; j <= 8; j++)
    {
      flag = tmp & 0x0001;
      tmp >>= 1;

      if (flag)
        tmp ^= 0xA001;
    }
  }

  tmp2 = tmp >> 8;
  tmp = (tmp << 8) | tmp2;
  tmp &= 0xFFFF;

  return tmp;
}
SoftwareSerial swSer(D2, D3, false, 256); //tx,rx


void setup() {
  swSer.begin(9600);
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
      rtu_len = sbuf[5];
      Serial.printf("rtu msg len %d \n\r" ,sbuf[5]);
      for (int i=0 ;i<readnum-6;i++){
        rtu_buf[i] = sbuf[i+6];
      }
      for (int i=0 ;i<rtu_len;i++){
      Serial.print(rtu_buf[i],HEX);
      }
      Serial.print("\n\r");
      int value = calcCRC(rtu_len); 
      Serial.println(value,HEX);
      rtu_buf[rtu_len]  = (value & 0xFF00) >> 8;
      rtu_buf[rtu_len+1] = value & 0x00FF;
      for (int i=0 ;i<rtu_len+2;i++){
      Serial.print(rtu_buf[i],HEX);
      }
      Serial.println();
      swSer.write(rtu_buf,rtu_len + 2);
      //next step , receive fron swSer
  }
 }
}
