#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Temp-Web.h"

#define ONE_WIRE_BUS 2  
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds(&oneWire);
float dsCurrTemp = 0;

const char* ssid = "TemperatureSensor";
const char* pass = "kamehameha";

// const char* ssid = "PLDTHOMEFIBR4fc38";
// const char* pass = "PLDTWIF!11b9b";

AsyncWebServer server(80);

void setup(void) {
  Serial.begin(115200);
  ds.begin();
  server_init();
}

void loop(void) {
  ds_GetTemp();
}

void ds_GetTemp() {
  // Serial.print("Requesting temperatures...");
  ds.requestTemperatures();
  dsCurrTemp = ds.getTempCByIndex(0);

  // if (dsCurrTemp != DEVICE_DISCONNECTED_C) {
  //   Serial.print("Temperature is: ");
  //   Serial.println(dsCurrTemp);
  // } else {
  //   Serial.println("Error: Could not read temperature data");
  // }
}

void server_init() {
  // WiFi.begin(ssid, pass);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.println("...");
  // }
  
  WiFi.softAP(ssid, pass);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  // Serial.println(WiFi.localIP());


  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", webpage);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Received: HTTP_GET /temperature");
    String response = "";
    if (dsCurrTemp != DEVICE_DISCONNECTED_C) {
      response = "Detected-" + String(dsCurrTemp);
    } else {
      response = "Not_Detected-0.0";
    }
    Serial.println("Responded with " + response);
    request->send(200, "text/plain", response);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/css", style_css);
  });

  // Start the server
  server.begin();
}