#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

// === Client Object ===
WiFiClient cl;
bool clIsConn = false;
bool temp = false;

void wifi_Init() {
  Serial.println("wifi_Init called");

  if(WiFi.getMode()!=WIFI_STA){
    WiFi.mode(WIFI_STA);
    WiFi.begin(STA_SSID, STA_PASS);  // Connect to WiFi using retrieved config variables
    temp = true;
    Serial.println("Reached: " + String(temp));
    Serial.println("Connecting to Wifi");
  }
  
  return;
}

void cl_Connect() {
  const char* host = STA_HOST.c_str();
  int port = STA_PORT.toInt();
  if (port == 0) port = 8080;
  Serial.println("Connecting to Server...");
  if (!cl.connect(host, port))
    Serial.println("Failed: Connection to " + String(host) + ":" + port);
  else
    Serial.println("Success: Connected to " + String(host) + ":" + port);
}

void cl_Stop(){
  if(cl.connected())  cl.stop();
}

#endif