#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <BH1750.h>
#include <Wire.h>
#include "Temp-Web.h"

String currLux = "none";

BH1750 lightMeter;
bool sensor_detected = false;

const char* ssid = "LightSensor";
const char* pass = "kamehameha";

AsyncWebServer server(80);

void setup(void) {
  Serial.begin(115200);
  sensor_init();
  server_init();
}

void loop(void) {
  if (!sensor_detected)
    sensor_Init();
  currLux = read_Sensor();  // "none" if not Detected
}

void sensor_init(){
  if(!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)){
    Serial.println(F("Sensor not detected"));
    sensor_detected = false;
  }else{
    Serial.println(F("Sensor initialized"));
    sensor_detected = true;
  }
}


String read_Sensor(){
  if(!sensor_detected){
    return "none";
  }

  unsigned long start_time = millis();

  while (!lightMeter.measurementReady(true)) {
    if(millis() - start_time == 5000){
      sensor_detected = false;
      return "none";
    }
  }
  float lux = lightMeter.readLightLevel();

  String value = String(lux);
  return value;
}

void server_init() {
  
  WiFi.softAP(ssid, pass);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/html", webpage);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("Received: HTTP_GET /temperature");
    String response = "";
    if (currLux != "none") {
      response = "Detected-" + currLux;
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