#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#ifndef STASSID
#define STASSID "SSID DEL WIFI"
#define STAPSK  "PASSWORD WIFI"
#endif

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1]; //buffer to hold incoming packet,
char  ReplyBuffer[120];       // a string to send back
unsigned char cont=0;
unsigned char SendCMD[8]={0x51,0x50,0x49,0x47,0x53,0xb7,0xa9,0x0d};
unsigned char SendCMDQMOD[7]={0x51,0x4d,0x4f,0x44,0x49,0xc1,0x0d};
unsigned char PktStart[5]={0x50,0x4B,0x54,0x5F,0x30};
unsigned char PktLINEA[3]={0x4C,0x4E,0x30};

unsigned char QMODELETTO=0;
unsigned LetturaQMODE=0;
WiFiUDP Udp;

void setup() {
  Serial.begin(2400);
  Serial1.begin(2400);
  Serial.setTimeout(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
   // Serial.print('.');
    delay(500);
  }
 /* Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  */
  Udp.begin(localPort);
  
}

void loop() {
  
  if (LetturaQMODE==0)
    LetturaQMODE=1;
  else
    LetturaQMODE=0;

  if (LetturaQMODE>0)
  {
     delay(1000);
     Serial.write(SendCMDQMOD, 7);
     String incomingString = Serial.readString();
     incomingString.toCharArray(ReplyBuffer, incomingString.length()+1);
     //QMODELETTO=ReplyBuffer[1];
     PktLINEA[2]=ReplyBuffer[1];
     Udp.beginPacket("255.255.255.255", 8888);
      
      Udp.write(PktLINEA,3);
      Udp.endPacket();
     
  }
  else
  {
    cont+=1;
    if (cont>9)
      cont=0;
  
    PktStart[4] = 0x30 + cont;
      
    delay(2000);
    Serial.write(SendCMD, 8);
    String incomingString = Serial.readString();
  
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.beginPacket("255.255.255.255", 8888);
    incomingString.toCharArray(ReplyBuffer, incomingString.length()+1);

    Udp.write(PktStart,5);
    Udp.write(ReplyBuffer,incomingString.length());
    Udp.endPacket();
  }
