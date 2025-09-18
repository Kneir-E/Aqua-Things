#include <WiFi.h>
#include <WiFiManager.h>    // https://github.com/tzapu/WiFiManager
#include <Preferences.h>
#include <String.h>

#include<ADS1115_WE.h> 
#include<Wire.h>

// NAMING SCHEME:
//  <_><object name(small letters)><number (optional)> - object (either from library or created locally)
//      ex:   _pf - preferences object
//            _adc0 - external adc no. 0
// 
//  <object name(small letters)><variable name (All Caps)><number (optional)>  - variable used by object or variable that uses object
//      ex:   pfSSID - variable that stores value from _pf object
//        
//  <object name(small letters)><_><function name (Pascal Case)> - function used by object
//      ex:   pf_Init() - function to initialize _pf object
// 
//  <object name(All Caps)><_c_><name(All Caps)> - macro relating to object
//      ex:   WM_TIMEOUT - macro used by WM object
// 
//  <function name (Camel Case)> - general function
//      ex:   

// === Variables and Objects: Preferences Library ===
  Preferences pf;
  // Wifi Credentials
  // These are global variables and will changed in pf_Init and upon WiFi Manager update
  String pfSSID = "";
  String pfPASS = "";
  String pfHOST = "";
  String pfPORT = "";
  bool pfEN0 = false;
  bool pfEN1 = false;
  bool pfEN2 = false;
  bool pfEN3 = false;
  bool pfEN4 = false;
  bool pfEN5 = false;
  bool pfEN6 = false;
  bool pfEN7 = false;

// === Variables and Constants: WiFi Manager ===
  #define WM_PIN 19        // Setup Mode Pin
  #define WM_TIMEOUT 120    // Setup Mode Time Limit
  bool wmSetupFlag = false;


// === Client Object ===
  WiFiClient cl;
  bool clIsConn = false;

// === External ADC Objects ===
  #define ADS_ADDGND 0x48
  #define ADS_ADDVDD 0x49
  ADS1115_WE _ads0 = ADS1115_WE(ADS_ADDGND);
  ADS1115_WE _ads1 = ADS1115_WE(ADS_ADDVDD);
  bool adsObj0 = false;
  bool adsObj1 = false;
  int adsCh0 = -1;
  int adsCh1 = -1;
  int adsCh2 = -1;
  int adsCh3 = -1;
  int adsCh4 = -1;
  int adsCh5 = -1;
  int adsCh6 = -1;
  int adsCh7 = -1;

void setup() {
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  wm_Init();    // Initialize Wifi Manager
  pf_Init();    // Retrieve Config Variables from ROM and store in RAM
  wifi_Init();
  ads_Init();
}

void loop() {
  
  // if SETUP_BUTTON is pressed, enter SETUP_MODE
  if(wmSetupFlag)
      wm_Setup();
  
  // if WiFi not Connected, Connect WiFi
  if(WiFi.status() != WL_CONNECTED)
      wifi_Init;

  // if WiFi connected but Client not connected, Connect Client
  if(WiFi.status() == WL_CONNECTED && !cl.connected())
    cl_conn();
  
  // if WiFi connected and Client connected, Handle comms with client
  else if(WiFi.status() == WL_CONNECTED && cl.connected()){ 
    // ========== HANDLE COMMS WITH SERVER  HERE==========
    if(cl.available()){
      // char *line = cl.readStringUntil('\r').c_str;
      // Serial.println("Received: " + String(line).c_str);
      
    }
    // String cl_msg = "DATA:";
    // if(pfEN0 && adsCh0>=0.0) cl_msg += ("CH0" + String(adsCh0) );
    // if(pfEN1 && adsCh1>=0.0) cl_msg += ("CH1" + String(adsCh1) );
    // if(pfEN2 && adsCh2>=0.0) cl_msg += ("CH2" + String(adsCh2) );
    // if(pfEN3 && adsCh3>=0.0) cl_msg += ("CH3" + String(adsCh3) );
    cl.write(String(adsCh0).c_str());
    Serial.println("Sent to Server: " + String(adsCh0));
  }

  if(adsObj0){
    adsCh0 = ads_Read(0, ADS1115_COMP_0_GND);
    adsCh1 = ads_Read(0, ADS1115_COMP_1_GND);
    adsCh2 = ads_Read(0, ADS1115_COMP_2_GND);
    adsCh3 = ads_Read(0, ADS1115_COMP_3_GND);
  }

  if(adsObj1){
    adsCh0 = ads_Read(1, ADS1115_COMP_0_GND);
    adsCh1 = ads_Read(1, ADS1115_COMP_1_GND);
    adsCh2 = ads_Read(1, ADS1115_COMP_2_GND);
    adsCh3 = ads_Read(1, ADS1115_COMP_3_GND);
  }

  // ========== FOREGROUND ROUTINE ==========
  Serial.println("In the loop...");
  delay(2000);
}


void wm_Init(){
  // Initialize pin for wifi manager setup
  pinMode(WM_PIN, INPUT_PULLUP);
  attachInterrupt(
    digitalPinToInterrupt(WM_PIN),
    wm_ISR,
    FALLING
  );
}

void wm_Setup(){
  // Stop Wifi Connection
  WiFi.disconnect(true, true);

  // Reset Setup Flag
  wmSetupFlag = false;

  // Create Wifi Manager object
  WiFiManager wm;
  WiFiManagerParameter host_ip("LSIP", "Host IP", pfHOST.c_str(), 16);
  WiFiManagerParameter host_port("LSPort", "Port", pfPORT.c_str(), 5);
  wm.addParameter(&host_ip);
  wm.addParameter(&host_port);

  //reset settings - for testing
  //wm.resetSettings();

  // set configportal timeout
  wm.setConfigPortalTimeout(WM_TIMEOUT);

  if (!wm.startConfigPortal("OnDemandAP","Password")) {
    Serial.println("Failed to Connect");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(5000);
  }else{
    Serial.println("Exitted");
    return;
  }

  // == SAVE THE OBTAINED DATA TO PREFERENCES == //
  // LOOK INTO CREATING A LIBRARY FOR THIS AND SAVING OBJECT
  pf_SetCred(wm.getWiFiSSID(), wm.getWiFiPass(), host_ip.getValue(), host_port.getValue());
  
  
  Serial.println("SSID: " + pfSSID);
  Serial.println("PASS: " + pfPASS);
  Serial.println("HOST: " + pfHOST + ":" + pfPORT);
  Serial.println("Config saved. Restarting device.");

  ESP.restart();
  return;
}

void wm_ISR(){
  wmSetupFlag = true;
}

void pf_Init(){
  pf.begin("Aqua-Things", false);

  // Initialize Variables
  pfSSID = pf.getString("SSID", "");
  pfPASS = pf.getString("PASS", "");
  pfHOST = pf.getString("HOST", "");
  pfPORT = pf.getString("PORT", "");
  
  return;
}

void pf_SetCred(String ssid, String pass, String host, String port){
  if( ssid != "" ){
    pfSSID = ssid;
    pf.putString("SSID", pfSSID);
  }
  
  if( pass != "" ){
    pfPASS = pass;
    pf.putString("PASS", pfPASS);
  }
  
  if( host != "" ){
    pfHOST = host;
    pf.putString("HOST", pfHOST);
  }

  if( port != "" ){
    pfPORT = port;
    pf.putString("PORT", pfPORT);
  }
}

void wifi_Init(){
  WiFi.begin(pfSSID, pfPASS);   // Connect to WiFi using retrieved config variables
  Serial.println("Connecting to Wifi");
  Serial.println("SSID: " + pfSSID);
  Serial.println("PASS: " + pfPASS);
  //WiFi.setAutoReconnect(true);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i <= 20) {
    delay(500);
    Serial.println("...");
    i++;
  }
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Wifi Connection Unsuccessful");
    return;
  }else                              
    Serial.println("Wifi Connected Successfully!");

  cl_conn();
  
  return;
}

void cl_conn(){
  const char* host = pfHOST.c_str();  
  int port = pfPORT.toInt();
  Serial.println("Connecting to Server...");
  if(!cl.connect(host, port)) 
    Serial.println("Failed: Connection to " + String(host) + ":" + port);
  else
    Serial.println("Success: Connected to " + String(host) + ":" + port);
}

void ads_Init(){
  // Initialize I2C protocol
  Wire.begin();

  // Initialize the first ADS1115
  if(!_ads0.init()){
    adsObj0 = false;
    Serial.println("External ADC0 (Left unit) not found!");
    return;
  }else{
    adsObj0 = true;
    _ads0.setVoltageRange_mV(ADS1115_RANGE_6144);
    Serial.println("External ADC0 (Left unit) verified!");
  }
  
  // Initialize the second ADS1115
  if(!_ads1.init()){
    adsObj1 = false;
    Serial.println("External ADC1 (Right unit) not found!");
    return;
  }else{
    adsObj1 = true;
    _ads1.setVoltageRange_mV(ADS1115_RANGE_6144);
    Serial.println("External ADC1 (Right unit) verified!");
  }  
}

int ads_Read(int adsNum, ADS1115_MUX channel) {
  ADS1115_WE _ads;
  if(adsNum==0)
    _ads = _ads0;
  else if(adsNum==1)
    _ads = _ads1;
  else return -1;

  int voltage = 0.0;
  _ads.setCompareChannels(channel);
  _ads.startSingleMeasurement();
  while(_ads.isBusy()){}
  voltage = _ads.getResult_mV(); // alternative: getResult_mV for Millivolt
  return voltage;
}
