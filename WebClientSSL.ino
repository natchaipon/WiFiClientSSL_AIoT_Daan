#include "WiFiEsp.h"

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "P";
char pass[] = "12345678";
int status = WL_IDLE_STATUS;

char server[] = "script.google.com";

WiFiEspClient client;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  WiFi.init(&Serial1);

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  printWifiStatus();
}

void loop() {
  int temp = random(100);
  int humi = random(100);
  
  if (client.connectSSL(server, 443)) {
    client.println("GET /macros/s/AKfycbyUx6-nk6j_5rynrT0tgDpgNZ8hpG5qYhkUVGaE0fLi3i5w_ftX/exec?temp=" + String(temp) + "&humi="  + String(humi) + " HTTP/1.1");
    client.println("Host: script.google.com");
    client.println("Connection: close");
    client.println();
  }
  
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  client.stop();
  delay(10000);
}


void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
