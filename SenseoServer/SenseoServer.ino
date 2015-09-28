/*
 * Senseo Kaffe Server
 * @author reinckm
 */

#define PGMSTR(x) (__FlashStringHelper*)(x)

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include "KaffeHtml.h"
#include "Credentials.h"

const char* host = "api.parse.com";
const char* fingerprint = "72 35 84 77 8B CE 18 57 79 D7 F2 69 CC B6 73 BF AA 31 D2 69";
const int httpsPort = 80;

// Arduino Interface
const int COFFE_PIN = 3; // High auf diesem Pin startet das Kaffeprogramm im Arduino

ESP8266WebServer server(80);

void handleRoot() {
  server.send_P(200, mime_html, index_html);
}

// COFFE_PIN für eine Sekunde auf High ziehen, so das der Arduino die Maschine steuern kann
void handleKaffe() {
  digitalWrite(COFFE_PIN, HIGH);
  delay(1000);
  digitalWrite(COFFE_PIN, LOW);
  server.send_P(200, mime_html, kaffee_html);
}

void setup() {
  pinMode(COFFE_PIN, OUTPUT);
  digitalWrite(COFFE_PIN, LOW);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  
  updateIP();
  pinMode(COFFE_PIN, OUTPUT);
  digitalWrite(COFFE_PIN, LOW);
  
  server.on("/", handleRoot);
  server.on("/kaffe", handleKaffe);
  server.onNotFound(handleRoot);
  server.begin();
}

void updateIP() {
  Serial.begin(9600);
  Serial.println(WiFi.localIP());
  WiFiClientSecure client;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  /**
   * Verify existiert in meiner Version nicht.
   * TODO: Prüfen und Einkommentieren
  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
  }
  */
  
  String url = "/1/classes/Device";
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: SenseoESP8266\r\n" +
               "X-Parse-Application-Id: " + parseAppId + "\r\n" +
               "X-Parse-REST-API-Key: " + parseApiKey + "\r\n" +
               "Content-Type: application/json\r\n"
               "Connection: close\r\n\r\n");
  client.print("{\"name\":\"Senseo\",\"ip\":\"\",\"primaryAction\":\"kaffe\"}");
  client.flush();
  
  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  
  String line = client.readStringUntil('\n');
  Serial.println(line);
  client.stop();
  Serial.end();
}


void loop() {
  server.handleClient();
  digitalWrite(COFFE_PIN, LOW);
}
