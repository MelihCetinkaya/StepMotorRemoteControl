#include "WiFi.h"
#include <WebServer.h>
#include <HTTPClient.h>

const char* ssid = "Oppox";
const char* password = "simon357";

WebServer server(8086);

int speedValue = 0;
int angleValue = 0;
int directionValue = 0;
int code;

void handleSetSpeed() {
  if (server.hasArg("value")) {
    String valueStr = server.arg("value");
    speedValue = valueStr.toInt();
    Serial.println("Gelen hız: " + String(speedValue));
    Serial2.print(speedValue);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Eksik parametre");
  }
}

void handleSetAngle() {
  if (server.hasArg("value")) {
    String valueStr = server.arg("value");
    angleValue = valueStr.toInt();
    Serial.println("Gelen açı: " + String(angleValue));
    Serial2.print(angleValue);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Eksik parametre");
  }
}

void handleSetDirection() {
  if (server.hasArg("value")) {
    String valueStr = server.arg("value");
    directionValue = valueStr.toInt();
    Serial.println("Gelen yön: " + String(directionValue));
    Serial2.print(directionValue);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Eksik parametre");
  }
}

void setup() {

Serial.begin(9600);
Serial2.begin(9600, SERIAL_8N1, 16, 17);  
delay(1000); 

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.println("Wifi'ya bağlanılıyor");
  }

  Serial.println("Wifi bağlantısı sağlandı");
  Serial.println("WiFi bağlandı, IP adresi: ");
  Serial.println(WiFi.localIP());
  

  server.on("/setSpeed", handleSetSpeed);
  server.on("/setAngle", handleSetAngle);
  server.on("/setDirection", handleSetDirection);
  server.begin();
  Serial.println("Sunucu başlatıldı.");
   
}

void loop() {

server.handleClient(); 


} 
